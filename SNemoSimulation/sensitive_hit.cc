// sensitive_hit.cc

// Standard library:
#include <iostream>
#include <sstream>
#include <stdexcept>

// Ourselves:
#include <SNemoSimulation/sensitive_hit.h>

namespace snemo {

  namespace g4 {

    const mctools::base_step_hit&
    sensitive_hit::get_hit_data() const
    {
      return _hit_data_;
    }

    mctools::base_step_hit&
    sensitive_hit::grab_hit_data()
    {
      return _hit_data_;
    }

    sensitive_hit::sensitive_hit() { return; }

    sensitive_hit::~sensitive_hit() { return; }

    void
    sensitive_hit::reset()
    {
      _hit_data_.reset();
      return;
    }

  } // end of namespace g4

} // end of namespace snemo
