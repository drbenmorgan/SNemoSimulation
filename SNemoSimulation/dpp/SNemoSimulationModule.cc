/* SNemoSimulationModule.cc */

// Ourselves:
#include "SNemoSimulationModule.h"

// Standard library:
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

// Third party:
// - Bayeux/datatools:
#include <datatools/exception.h>
#include <datatools/ioutils.h>
#include <datatools/properties.h>
#include <datatools/service_manager.h>
#include <datatools/utils.h>
// - Bayeux/geomtools:
#include <geomtools/geometry_service.h>
// - Bayeux/mctools:
#include <mctools/simulated_data.h>
#include <mctools/utils.h>

// This project:
#include <SNemoSimulation/event_action.h>
#include <SNemoSimulation/manager.h>
#include <SNemoSimulation/simulation_ctrl.h>

namespace snemo {
  namespace g4 {

    // Registration instantiation macro :
    DPP_MODULE_REGISTRATION_IMPLEMENT(SNemoSimulationModule, "snemo::g4::SNemoSimulationModule")

    // Constructor :
    SNemoSimulationModule::SNemoSimulationModule(datatools::logger::priority logging_priority)
      : dpp::base_module(logging_priority)
    {
      geometryServiceName_ = "";
      simdataBankName_ = "";
      geometryManagerRef_ = 0;
      geant4Simulation_ = 0;
      geant4SimulationController_ = 0;
    }

    // Destructor :
    SNemoSimulationModule::~SNemoSimulationModule()
    {
      // Make sure all internal resources are terminated
      // before destruction :
      if (is_initialized())
        reset();
    }

    // Initialization :
    void
    SNemoSimulationModule::initialize(const datatools::properties& config_,
                                      datatools::service_manager& service_manager_,
                                      dpp::module_handle_dict_type& /*module_dict_*/)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Module '" << get_name() << "' is already initialized !");

      // Parsing configuration starts here :
      this->_common_initialize(config_);

      if (geometryServiceName_.empty()) {
        if (config_.has_key("Geo_label")) {
          geometryServiceName_ = config_.fetch_string("Geo_label");
        }
      }

      if (simdataBankName_.empty()) {
        if (config_.has_key("SD_label")) {
          simdataBankName_ = config_.fetch_string("SD_label");
        }
      }

      // Special setup parameters for the snemo::g4 simulation manager :

      // Force non-interactive parameters:
      geant4Parameters_.interactive = false;
      geant4Parameters_.g4_visu = false;
      geant4Parameters_.g4_macro = "";
      geant4Parameters_.output_data_file.clear();

      if (geant4Parameters_.logging == "fatal") {
        if (config_.has_flag("manager.logging.priority")) {
          geant4Parameters_.logging = config_.fetch_string("manager.logging.priority");
        } else if (config_.has_flag("manager.debug")) {
          geant4Parameters_.logging = "debug";
        } else if (config_.has_flag("manager.verbose")) {
          geant4Parameters_.logging = "notice";
        }
      }

      if (geant4Parameters_.manager_config_filename.empty()) {
        if (config_.has_key("manager.configuration_filename")) {
          geant4Parameters_.manager_config_filename =
            config_.fetch_string("manager.configuration_filename");
        }
      }

      if (geant4Parameters_.mgr_seed == mygsl::random_utils::SEED_INVALID) {
        if (config_.has_key("manager.seed")) {
          geant4Parameters_.mgr_seed = config_.fetch_integer("manager.seed");
        }
      }

      if (geant4Parameters_.vg_name.empty()) {
        if (config_.has_key("manager.vertex_generator_name")) {
          geant4Parameters_.vg_name = config_.fetch_string("manager.vertex_generator_name");
        }
      }

      if (geant4Parameters_.vg_seed == mygsl::random_utils::SEED_INVALID) {
        if (config_.has_key("manager.vertex_generator_seed")) {
          geant4Parameters_.vg_seed = config_.fetch_integer("manager.vertex_generator_seed");
        }
      }

      if (geant4Parameters_.eg_name.empty()) {
        if (config_.has_key("manager.event_generator_name")) {
          geant4Parameters_.eg_name = config_.fetch_string("manager.event_generator_name");
        }
      }

      if (geant4Parameters_.eg_seed == mygsl::random_utils::SEED_INVALID) {
        if (config_.has_key("manager.event_generator_seed")) {
          geant4Parameters_.eg_seed = config_.fetch_integer("manager.event_generator_seed");
        }
      }

      if (geant4Parameters_.shpf_seed == mygsl::random_utils::SEED_INVALID) {
        if (config_.has_key("manager.shpf_seed")) {
          geant4Parameters_.shpf_seed = config_.fetch_integer("manager.shpf_seed");
        }
      }

      if (geant4Parameters_.input_prng_seeds_file.empty()) {
        if (config_.has_key("manager.input_prng_seeds_file")) {
          geant4Parameters_.input_prng_seeds_file =
            config_.fetch_string("manager.input_prng_seeds_file");
        }
      }

      if (geant4Parameters_.output_prng_seeds_file.empty()) {
        if (config_.has_key("manager.output_prng_seeds_file")) {
          geant4Parameters_.output_prng_seeds_file =
            config_.fetch_string("manager.output_prng_seeds_file");
        }
      }

      if (geant4Parameters_.input_prng_states_file.empty()) {
        if (config_.has_key("manager.input_prng_states_file")) {
          geant4Parameters_.input_prng_states_file =
            config_.fetch_string("manager.input_prng_states_file");
        }
      }

      if (geant4Parameters_.output_prng_states_file.empty()) {
        if (config_.has_key("manager.output_prng_states_file")) {
          geant4Parameters_.output_prng_states_file =
            config_.fetch_string("manager.output_prng_states_file");
        }
      }

      if (geant4Parameters_.prng_states_save_modulo == 0) {
        if (config_.has_key("manager.prng_states_save_modulo")) {
          geant4Parameters_.prng_states_save_modulo =
            config_.fetch_integer("manager.prng_states_save_modulo");
        }
      }

      if (geant4Parameters_.number_of_events_modulo == 0) {
        if (config_.has_key("manager.number_of_events_modulo")) {
          geant4Parameters_.number_of_events_modulo =
            config_.fetch_integer("manager.number_of_events_modulo");
        }
      }

      // More setup parameters can be added.

      if (get_logging_priority() >= datatools::logger::PRIO_TRACE) {
        geant4Parameters_.tree_dump(std::clog, "", "[trace]: ");
      }
      // Parsing configuration stops here .

      // Initialization starts here :

      if (geometryManagerRef_ == 0) {
        // Access to the Geometry Service :
        DT_THROW_IF(geometryServiceName_.empty(),
                    std::logic_error,
                    "Module '" << get_name() << "' has no valid '"
                               << "Geo_label"
                               << "' property !");
        if (service_manager_.has(geometryServiceName_) &&
            service_manager_.is_a<geomtools::geometry_service>(geometryServiceName_)) {
          // Fetch a reference to the geometry service :
          geomtools::geometry_service& Geo =
            service_manager_.grab<geomtools::geometry_service>(geometryServiceName_);
          // Request for a reference to the geometry manager and installation
          // in the simulation manager :
          this->set_geometry_manager(Geo.get_geom_manager());
        } else {
          DT_THROW_IF(true,
                      std::logic_error,
                      "Module '" << get_name() << "' has no '" << geometryServiceName_
                                 << "' geometry service !");
        }
      }

      DT_THROW_IF(geometryManagerRef_ == 0, std::logic_error, "Missing geometry manager !");

      if (simdataBankName_.empty()) {
        // Use the default label for the 'simulated data' bank :
        simdataBankName_ = ::mctools::event_utils::EVENT_DEFAULT_SIMULATED_DATA_LABEL; // "SD"
      }

      // Allocate internal resources :
      this->_initialize_manager(service_manager_);
      this->_set_initialized(true);
    }

    // Reset :
    void
    SNemoSimulationModule::reset()
    {
      DT_THROW_IF(
        !is_initialized(), std::logic_error, "Module '" << get_name() << "' is not initialized !");

      _set_initialized(false);

      if (geant4SimulationController_ != 0) {
        // Destruction of the thread synchronization object :
        geant4SimulationController_->set_stop_requested();
        delete geant4SimulationController_;
        geant4SimulationController_ = 0;
      }

      // Destroy internal resources :
      _terminate_manager();

      // Blank the module with default neutral values :
      geometryManagerRef_ = 0;
      simdataBankName_ = "";
      geometryServiceName_ = "";
    }

    // Processing :
    dpp::base_module::process_status
    SNemoSimulationModule::process(datatools::things& event_record_)
    {
      DT_THROW_IF(!this->is_initialized(),
                  std::logic_error,
                  "Module '" << get_name() << "' is not initialized !");
      int status = this->_simulate_event(event_record_);
      return status == 0 ? dpp::base_module::PROCESS_OK : dpp::base_module::PROCESS_FATAL;
    }

    void
    SNemoSimulationModule::set_geo_label(const std::string& geo_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Module '" << get_name() << "' is already initialized !");
      geometryServiceName_ = geo_;
    }

    const std::string&
    SNemoSimulationModule::get_geo_label() const
    {
      return geometryServiceName_;
    }

    void
    SNemoSimulationModule::set_sd_label(const std::string& sd_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Module '" << get_name() << "' is already initialized !");
      simdataBankName_ = sd_;
    }

    const std::string&
    SNemoSimulationModule::get_sd_label() const
    {
      return simdataBankName_;
    }

    void
    SNemoSimulationModule::set_geometry_manager(const geomtools::manager& geometry_manager_)
    {
      DT_THROW_IF(geometryManagerRef_ != 0 && geometryManagerRef_->is_initialized(),
                  std::logic_error,
                  "Embedded geometry manager is already initialized !");
      geometryManagerRef_ = &geometry_manager_;
    }

    void
    SNemoSimulationModule::set_geant4_parameters(const manager_parameters& params_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Module '" << get_name() << "' is already initialized !");
      geant4Parameters_ = params_;
    }

    const manager_parameters&
    SNemoSimulationModule::get_geant4_parameters() const
    {
      return geant4Parameters_;
    }

    const mygsl::seed_manager&
    SNemoSimulationModule::get_seed_manager() const
    {
      DT_THROW_IF(
        !is_initialized(), std::logic_error, "Module '" << get_name() << "' is not initialized !");
      return geant4Simulation_->get_seed_manager();
    }

    const mygsl::prng_state_manager&
    SNemoSimulationModule::get_state_manager() const
    {
      DT_THROW_IF(
        !is_initialized(), std::logic_error, "Module '" << get_name() << "' is not initialized !");
      return geant4Simulation_->get_state_manager();
    }

    void
    SNemoSimulationModule::_initialize_manager(datatools::service_manager& smgr_)
    {
      // Allocate the simulation manager :
      geant4Simulation_ = new manager;

      // 2012-04-30 FM: add support for a handle to the service manager
      geant4Simulation_->set_service_manager(smgr_);

      // Install the geometry manager accessed from the
      // Geometry service (bypassing the embedded geometry manager
      // in the simulation manager) :
      geant4Simulation_->set_external_geom_manager(*geometryManagerRef_);

      // Install a simulation controler in the manager :
      if (geant4SimulationController_ == 0) {
        geant4SimulationController_ = new simulation_ctrl(*geant4Simulation_);
        geant4Simulation_->set_simulation_ctrl(*geant4SimulationController_);
      }

      // Setup :
      snemo::g4::manager_parameters::setup(geant4Parameters_, *geant4Simulation_);
    }

    void
    SNemoSimulationModule::_terminate_manager()
    {
      if (geant4SimulationController_ != 0) {
        delete geant4SimulationController_;
        geant4SimulationController_ = 0;
      }

      if (geant4Simulation_ != 0) {
        delete geant4Simulation_;
        geant4Simulation_ = 0;
      }

      geant4Parameters_.reset();
    }

    int
    SNemoSimulationModule::_simulate_event(datatools::things& workItem)
    {
      // Bank name must be unique
      DT_THROW_IF(workItem.has(simdataBankName_),
                  std::runtime_error,
                  "Work item input to module '"
                    << this->get_name() << "' already has data bank named '" << simdataBankName_
                    << "'");
      auto& sdBank = workItem.add<mctools::simulated_data>(simdataBankName_);
      geant4Simulation_->grab_user_event_action().set_external_event_data(sdBank);

      {
        {
          boost::mutex::scoped_lock lock(*geant4SimulationController_->event_mutex);

          if (geant4SimulationController_->simulation_thread == 0) {
            geant4SimulationController_->start();
          }
          geant4SimulationController_->event_availability_status =
            simulation_ctrl::AVAILABLE_FOR_G4;
          geant4SimulationController_->event_available_condition->notify_one();
        }

        // Wait for the release of the event control by the G4 process :
        {
          boost::mutex::scoped_lock lock(*geant4SimulationController_->event_mutex);

          while (geant4SimulationController_->event_availability_status ==
                 simulation_ctrl::AVAILABLE_FOR_G4) {
            geant4SimulationController_->event_available_condition->wait(
              *geant4SimulationController_->event_mutex);
          }
        }
      }
      return 0;
    }
  } // end of namespace g4
} // end of namespace snemo
/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
