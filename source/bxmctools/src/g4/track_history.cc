// -*- mode: c++ ; -*-
/* track_history.cc
 */

#include <mctools/g4/track_history.h>

#include <sstream>
#include <stdexcept>

namespace mctools {

  namespace g4 {

    const int track_history::track_info::TRACK_ID_UNSET = 0;

    int track_history::track_info::get_id () const
    {
      return _id_;
    }

    void track_history::track_info::set_id (const int id_)
    {
      _id_ = id_;
      return;
    }

    int track_history::track_info::get_parent_id () const
    {
      return _parent_id_;
    }

    void track_history::track_info::set_parent_id (const int id_)
    {
      _parent_id_ = id_;
      return;
    }

    bool track_history::track_info::is_primary () const
    {
      return _parent_id_ == TRACK_ID_UNSET;
    }

    const std::string & track_history::track_info::get_particle_name () const
    {
      return _particle_name_;
    }

    void track_history::track_info::set_particle_name (const std::string & name_)
    {
      _particle_name_ = name_;
    }

    const std::string & track_history::track_info::get_creator_process_name () const
    {
      return _creator_process_name_;
    }

    void track_history::track_info::set_creator_process_name (const std::string & name_)
    {
      _creator_process_name_ = name_;
    }

    const std::string & track_history::track_info::get_creator_sensitive_category () const
    {
      return _creator_sensitive_category_;
    }

    void track_history::track_info::set_creator_sensitive_category (const std::string & category_)
    {
      _creator_sensitive_category_ = category_;
    }

    void track_history::track_info::reset ()
    {
      _id_        = TRACK_ID_UNSET;
      _parent_id_ = TRACK_ID_UNSET;
    }

    track_history::track_info::track_info ()
    {
      reset ();
      return;
    }

    track_history::track_info::~track_info ()
    {
      reset ();
      return;
    }

    const track_history::track_info_dict_type & track_history::get_track_infos () const
    {
      return _track_infos_;
    }

    track_history::track_info_dict_type & track_history::grab_track_infos ()
    {
      return _track_infos_;
    }

    bool track_history::has_track_info (const int id_) const
    {
      return _track_infos_.find (id_) != _track_infos_.end ();
    }

    const track_history::track_info & track_history::get_track_info (const int id_) const
    {
      if (! has_track_info (id_))
        {
          std::ostringstream message;
          message << "mctools::g4::track_history::get_track_info: "
                  << "No track with id " << id_ << " has been stored!";
          throw std::logic_error (message.str ());
        }

      return _track_infos_.at (id_);
    }

    track_history::track_info & track_history::grab_track_info (const int id_)
    {
      if (! has_track_info (id_))
        {
          std::ostringstream message;
          message << "mctools::g4::track_history::grab_track_info: "
                  << "No track with id " << id_ << " has been stored!";
          throw std::logic_error (message.str ());
        }

      return _track_infos_[id_];
    }

    void track_history::add_track_info (const int id_, const track_info & tinfo_)
    {
      if (has_track_info (id_))
        {
          std::ostringstream message;
          message << "mctools::g4::track_history::grab_track_info: "
                  << "A track with id '" << id_ << "' already exist!";
          throw std::logic_error (message.str ());
        }
      _track_infos_[id_] = tinfo_;
    }

    track_history::track_history ()
    {
      return;
    }

    track_history::~track_history ()
    {
      reset ();
      return;
    }

    void track_history::reset ()
    {
      _track_infos_.clear ();
      return;
    }

  }  // end of namespace g4

}  // end of namespace mctools

// end of track_history.cc
/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** End: --
*/
