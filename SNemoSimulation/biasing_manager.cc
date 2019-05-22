/// \file mctools/biasing_manager.cc

// Ourselves:
#include <SNemoSimulation/biasing_manager.h>

// - Bayeux/datatools :
#include <datatools/exception.h>
#include <datatools/properties.h>
#include <datatools/utils.h>

// This project:
#include <SNemoSimulation/detector_construction.h>

namespace snemo {

  namespace g4 {

    biasing_manager::biasing_manager()
    {
      _set_default();
      return;
    }

    biasing_manager::~biasing_manager() { return; }

    void
    biasing_manager::set_detector_construction(detector_construction& dc_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Biasing manager is locked!");
      _dc_ = &dc_;
      return;
    }

    void
    biasing_manager::_set_default()
    {
      _initialized_ = false;
      _dc_ = 0;
      return;
    }

    bool
    biasing_manager::is_initialized() const
    {
      return _initialized_;
    }

    void
    biasing_manager::initialize(const datatools::properties& /*config_*/)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Biasing manager is already initialized!");

      // Do something

      _initialized_ = true;
      return;
    }

    // Reset
    void
    biasing_manager::reset()
    {
      DT_THROW_IF(!is_initialized(), std::logic_error, "Biasing manager is not initialized!");
      _initialized_ = false;

      // Do something

      return;
    }

  } // end of namespace g4

} // end of namespace snemo
