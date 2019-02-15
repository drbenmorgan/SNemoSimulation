cmake_minimum_required(VERSION 3.8)
project(SNemoSimulation VERSION 3.3.0)

# Need Bayeux and Geant4
# Use exact versions for now to check compilation/run
find_package(Bayeux 3.3.0 EXACT REQUIRED)
find_package(Geant4 9.6.4 EXACT REQUIRED)

# Sources etc.
configure_file(
  source/bxmctools/include/mctools/g4/data_libraries.h.in
  source/bxmctools/include/mctools/g4/data_libraries.h
  @ONLY
  )

set(SNemoSimulation_HEADERS
  ${PROJECT_BINARY_DIR}/source/bxmctools/include/mctools/g4/data_libraries.h
  source/bxmctools/include/mctools/g4/tracking_action.h
  source/bxmctools/include/mctools/g4/particles_physics_constructor.h
  source/bxmctools/include/mctools/g4/stacking_action.h
  source/bxmctools/include/mctools/g4/simulation_module.h
  source/bxmctools/include/mctools/g4/simulation_ctrl.h
  source/bxmctools/include/mctools/g4/em_field_g4_utils.h
  source/bxmctools/include/mctools/g4/track_history.h
  source/bxmctools/include/mctools/g4/manager.h
  source/bxmctools/include/mctools/g4/processes/utils.h
  source/bxmctools/include/mctools/g4/processes/em_model_factory.h
  source/bxmctools/include/mctools/g4/processes/em_extra_models.h
  source/bxmctools/include/mctools/g4/detector_construction.h
  source/bxmctools/include/mctools/g4/physics_list.h
  source/bxmctools/include/mctools/g4/sensitive_hit_collection.h
  source/bxmctools/include/mctools/g4/loggable_support.h
  source/bxmctools/include/mctools/g4/em_physics_constructor.h
  source/bxmctools/include/mctools/g4/em_field_g4_stuff.h
  source/bxmctools/include/mctools/g4/electromagnetic_field.h
  source/bxmctools/include/mctools/g4/biasing_manager.h
  source/bxmctools/include/mctools/g4/g4_prng.h
  source/bxmctools/include/mctools/g4/region_tools.h
  source/bxmctools/include/mctools/g4/magnetic_field.h
  source/bxmctools/include/mctools/g4/neutrons_physics_constructor.h
  source/bxmctools/include/mctools/g4/primary_generator.h
  source/bxmctools/include/mctools/g4/run_action.h
  source/bxmctools/include/mctools/g4/stepping_verbose.h
  source/bxmctools/include/mctools/g4/sensitive_detector.h
  source/bxmctools/include/mctools/g4/manager_parameters.h
  source/bxmctools/include/mctools/g4/physics_list_utils.h
  source/bxmctools/include/mctools/g4/sensitive_hit.h
  source/bxmctools/include/mctools/g4/base_physics_constructor.h
  source/bxmctools/include/mctools/g4/stepping_action.h
  source/bxmctools/include/mctools/g4/em_field_equation_of_motion.h
  source/bxmctools/include/mctools/g4/event_action.h
  )

set(SNemoSimulation_SOURCES
  source/bxmctools/src/g4/physics_list.cc
  source/bxmctools/src/g4/stacking_action.cc
  source/bxmctools/src/g4/neutrons_physics_constructor.cc
  source/bxmctools/src/g4/simulation_module.cc
  source/bxmctools/src/g4/em_physics_constructor.cc
  source/bxmctools/src/g4/base_physics_constructor.cc
  source/bxmctools/src/g4/sensitive_hit.cc
  source/bxmctools/src/g4/physics_list_utils.cc
  source/bxmctools/src/g4/manager.cc
  source/bxmctools/src/g4/event_action.cc
  source/bxmctools/src/g4/stepping_action.cc
  source/bxmctools/src/g4/track_history.cc
  source/bxmctools/src/g4/processes/utils.cc
  source/bxmctools/src/g4/processes/em_model_factory.cc
  source/bxmctools/src/g4/processes/em_extra_models.cc
  source/bxmctools/src/g4/run_action.cc
  source/bxmctools/src/g4/region_tools.cc
  source/bxmctools/src/g4/sensitive_detector.cc
  source/bxmctools/src/g4/loggable_support.cc
  source/bxmctools/src/g4/biasing_manager.cc
  source/bxmctools/src/g4/tracking_action.cc
  source/bxmctools/src/g4/particles_physics_constructor.cc
  source/bxmctools/src/g4/primary_generator.cc
  source/bxmctools/src/g4/simulation_ctrl.cc
  source/bxmctools/src/g4/magnetic_field.cc
  source/bxmctools/src/g4/electromagnetic_field.cc
  source/bxmctools/src/g4/detector_construction.cc
  source/bxmctools/src/g4/g4_prng.cc
  source/bxmctools/src/g4/manager_parameters.cc
  source/bxmctools/src/g4/sensitive_hit_collection.cc
  source/bxmctools/src/g4/em_field_equation_of_motion.cc
  source/bxmctools/src/g4/em_field_g4_stuff.cc
  source/bxmctools/src/g4/stepping_verbose.cc
  )

# Add library
add_library(SNemoSimulation SHARED ${SNemoSimulation_HEADERS} ${SNemoSimulation_SOURCES})
target_include_directories(SNemoSimulation
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source/bxmctools/include>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/source/bxmctools/include>
    ${Geant4_INCLUDE_DIRS}
    )
target_link_libraries(SNemoSimulation Bayeux::Bayeux ${Geant4_LIBRARIES})

# As a loadable module, probably only need to install
# lib(s) (Module interface + implementation library).
