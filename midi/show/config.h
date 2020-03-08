#pragma once

#include <QColor>
#include <memory>
#include <string>
#include "midi/show/singleton.h"

namespace midi {

constexpr int NUM_PIANO_KEYS = 88;
constexpr int NUM_PIANO_WHITE_KEYS = 52;
constexpr int NUM_WHITE_KEYS_PER_GROUP = 7;
constexpr int MIN_NOTE_ON_KEYBOARD = 21;
constexpr int MAX_NOTE_ON_KEYBOARD = 108;

class Theater;
class Stage;
class Director;

class Notes;

class Config {
 public:
  // widgets
  Theater *theater_;
  Stage *stage_;
  Director *director_;

  // file
  std::string binary_path_;
  std::shared_ptr<Notes> notes_;

  // timestamp
  int64_t timestamp_reserve_;
  int64_t timestamp_base_;
  int64_t timestamp_;

  // stage
  int64_t time_scope_;
  float fps_;
  bool is_white_key_[NUM_PIANO_KEYS];
  float key_map_[NUM_PIANO_KEYS];
  float width_white_key_;
  float width_black_key_;
  float height_white_key_;
  float height_black_key_;

  // background
  float background_alpha_;

  // particle
  QColor particle_color_;
  float particle_alpha_;
  int64_t particle_lifetime_;
  size_t particle_limit_;
  int particle_size_;

  // note
  QColor note_color_;
  float note_alpha_;

  // keyboard
  bool show_keyboard_;
  QColor keyboard_color_;

 private:
  Config();
  ~Config();
  friend class Singleton<Config>;
};

}  // namespace midi
