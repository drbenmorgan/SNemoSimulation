cmake_minimum_required(VERSION 3.8)
project(SNemoSimulation VERSION 3.3.0)

# Need Bayeux and Geant4
find_package(Bayeux 3.3.0 REQUIRED)
find_package(Geant4 9.6.4 REQUIRED)



# Sources etc.
configure_file(
  SNemoSimulation/data_libraries.h.in
  SNemoSimulation/data_libraries.h
  @ONLY
  )

set(SNemoSimulationCore_SOURCES
  ${PROJECT_BINARY_DIR}/SNemoSimulation/data_libraries.h
  SNemoSimulation/manager.h
  SNemoSimulation/manager.cc
  SNemoSimulation/manager_parameters.h
  SNemoSimulation/manager_parameters.cc



  SNemoSimulation/tracking_action.h
  SNemoSimulation/particles_physics_constructor.h
  SNemoSimulation/stacking_action.h
  SNemoSimulation/simulation_ctrl.h
  SNemoSimulation/em_field_g4_utils.h
  SNemoSimulation/track_history.h
  SNemoSimulation/processes/utils.h
  SNemoSimulation/processes/em_model_factory.h
  SNemoSimulation/processes/em_extra_models.h
  SNemoSimulation/detector_construction.h
  SNemoSimulation/physics_list.h
  SNemoSimulation/sensitive_hit_collection.h
  SNemoSimulation/loggable_support.h
  SNemoSimulation/em_physics_constructor.h
  SNemoSimulation/em_field_g4_stuff.h
  SNemoSimulation/electromagnetic_field.h
  SNemoSimulation/biasing_manager.h
  SNemoSimulation/g4_prng.h
  SNemoSimulation/region_tools.h
  SNemoSimulation/magnetic_field.h
  SNemoSimulation/neutrons_physics_constructor.h
  SNemoSimulation/primary_generator.h
  SNemoSimulation/run_action.h
  SNemoSimulation/stepping_verbose.h
  SNemoSimulation/sensitive_detector.h
  SNemoSimulation/physics_list_utils.h
  SNemoSimulation/sensitive_hit.h
  SNemoSimulation/base_physics_constructor.h
  SNemoSimulation/stepping_action.h
  SNemoSimulation/em_field_equation_of_motion.h
  SNemoSimulation/event_action.h
  SNemoSimulation/physics_list.cc
  SNemoSimulation/stacking_action.cc
  SNemoSimulation/neutrons_physics_constructor.cc
  SNemoSimulation/em_physics_constructor.cc
  SNemoSimulation/base_physics_constructor.cc
  SNemoSimulation/sensitive_hit.cc
  SNemoSimulation/physics_list_utils.cc
  SNemoSimulation/event_action.cc
  SNemoSimulation/stepping_action.cc
  SNemoSimulation/track_history.cc
  SNemoSimulation/processes/utils.cc
  SNemoSimulation/processes/em_model_factory.cc
  SNemoSimulation/processes/em_extra_models.cc
  SNemoSimulation/run_action.cc
  SNemoSimulation/region_tools.cc
  SNemoSimulation/sensitive_detector.cc
  SNemoSimulation/loggable_support.cc
  SNemoSimulation/biasing_manager.cc
  SNemoSimulation/tracking_action.cc
  SNemoSimulation/particles_physics_constructor.cc
  SNemoSimulation/primary_generator.cc
  SNemoSimulation/simulation_ctrl.cc
  SNemoSimulation/magnetic_field.cc
  SNemoSimulation/electromagnetic_field.cc
  SNemoSimulation/detector_construction.cc
  SNemoSimulation/g4_prng.cc
  SNemoSimulation/sensitive_hit_collection.cc
  SNemoSimulation/em_field_equation_of_motion.cc
  SNemoSimulation/em_field_g4_stuff.cc
  SNemoSimulation/stepping_verbose.cc
  )

# Add implementation library
add_library(SNemoSimulationCore SHARED ${SNemoSimulationCore_SOURCES})
target_include_directories(SNemoSimulationCore
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
    ${Geant4_INCLUDE_DIRS}
    )
target_link_libraries(SNemoSimulationCore Bayeux::Bayeux ${Geant4_LIBRARIES})

# Add DPP module library
add_library(SNemoSimulationModule SHARED
  SNemoSimulation/dpp/SNemoSimulationModule.h
  SNemoSimulation/dpp/SNemoSimulationModule.cc
  )
target_link_libraries(SNemoSimulationModule PRIVATE SNemoSimulationCore)

# As a loadable module, probably only need to install
# lib(s) (Module interface + implementation library).
