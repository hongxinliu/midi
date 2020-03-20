#include "midi/show/config.h"
#include <boost/filesystem.hpp>
#include "midi/show/binary_path.h"

namespace midi {

Config::Config() {
  // get binary path
  binary_path_ = boost::filesystem::path(binary_path())
                     .remove_filename()
                     .remove_leaf()
                     .string();

  // timestamp
  timestamp_reserve_ = 0;
  timestamp_base_ = timestamp_reserve_;
  timestamp_ = timestamp_reserve_;

  // stage
  time_scope_ = 5000000;
  fps_ = 60.f;
  is_white_key_[0] = is_white_key_[2] = is_white_key_[3] = is_white_key_[5] =
      is_white_key_[7] = is_white_key_[8] = is_white_key_[10] = true;
  is_white_key_[1] = is_white_key_[4] = is_white_key_[6] = is_white_key_[9] =
      is_white_key_[11] = false;
  for (int i = 12; i < NUM_PIANO_KEYS; ++i) {
    is_white_key_[i] = is_white_key_[i - 12];
  }

  // background
  background_alpha_ = 0.3f;

  // particle
  particle_trail_ = true;
  particle_color_.setRgbF(1, 1, 1);
  particle_alpha_ = 0.4f;
  particle_lifetime_ = 3000000;
  particle_limit_ = 512;
  particle_size_ = 5;

  // note
  note_color_.setRgbF(1, 1, 1);
  note_alpha_ = 0.8f;
  note_rounded_radius_ = 8.f;

  // keyboard
  show_keyboard_ = true;
  keyboard_color_.setRgbF(0.125490, 0.290196, 0.470588);
}

Config::~Config() {}

}  // namespace midi
