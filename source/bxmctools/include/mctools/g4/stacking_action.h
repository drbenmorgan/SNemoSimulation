// -*- mode: c++ ; -*-
/* stacking_action.h
 * Author (s) :     Francois Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date: 2010-04-10
 * Last modified: 2013-03-09
 *
 * License:
 *
 * Description:
 *   G4 user stacking action class
 *
 * History:
 *
 */

#ifndef MCTOOLS_G4_STACKING_ACTION_H_
#define MCTOOLS_G4_STACKING_ACTION_H_ 1

#include <string>
#include <vector>

// G4 stuff:
#include <G4UserStackingAction.hh>

#include <mctools/g4/loggable_support.h>

class G4Track;
class G4LogicalVolume;
class G4Navigator;
class G4Material;

namespace datatools {
  class properties;
}

namespace mctools {

  namespace g4 {

    class stacking_action : public G4UserStackingAction,
                            public loggable_support
    {
    public:

      stacking_action ();

      virtual ~stacking_action ();

      void initialize (const datatools::properties & config_);

      // Geant4 interface :
      virtual G4ClassificationOfNewTrack ClassifyNewTrack (const G4Track * track_);

      virtual void NewStage ();

      virtual void PrepareNewEvent ();

    private:

      G4Navigator *                  _g4_navigator_;
      bool                           _kill_secondary_particles_;
      std::vector<std::string>       _killer_volume_names_;
      std::vector<G4LogicalVolume *> _killer_volumes_;
      std::vector<std::string>       _killer_material_names_;
      std::vector<G4Material *>      _killer_materials_;

    };

  } // end of namespace g4

} // end of namespace mctools

/// OCD support : interface
#include <datatools/ocd_macros.h>
DOCD_CLASS_DECLARATION(mctools::g4::stacking_action)

#endif // MCTOOLS_G4_STACKING_ACTION_H_

// end of stacking_action.h
