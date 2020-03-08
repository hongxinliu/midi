#include "midi/show/actors/actor_keyboard.h"
#include "midi/proto/note.pb.h"
#include "midi/show/config.h"
#include "midi/show/stage.h"
#include "midi/show/theater.h"

namespace midi {

void ActorKeyboard::perform() {
  if (!config_->show_keyboard_) {
    return;
  }

  glDisable(GL_BLEND);

  // flush background
  glColor4f(0, 0, 0, 1);
  drawRectangle(0, config_->stage_->height() - config_->height_white_key_,
                config_->stage_->width(), config_->height_white_key_);

  // draw white keys
  glColor4f(1, 1, 1, 1);
  for (int i = 0; i < NUM_PIANO_KEYS; ++i) {
    if (!config_->is_white_key_[i]) {
      continue;
    }
    float w = config_->width_white_key_;
    float h = config_->height_white_key_;
    float x = config_->key_map_[i] - w * 0.5f;
    float y = config_->stage_->height() - config_->height_white_key_;
    drawRectangle(x, y, w, h);
  }

  // draw events of white keys
  glColor4f(config_->keyboard_color_.redF(), config_->keyboard_color_.greenF(),
            config_->keyboard_color_.blueF(), 1);
  if (config_->notes_) {
    for (const auto &note : config_->notes_->notes()) {
      if (note.note() < MIN_NOTE_ON_KEYBOARD ||
          note.note() > MAX_NOTE_ON_KEYBOARD) {
        continue;
      }

      const auto key = note.note() - MIN_NOTE_ON_KEYBOARD;
      if (!config_->is_white_key_[key]) {
        continue;
      }

      if (note.timestamp() <= config_->timestamp_ &&
          note.timestamp() + note.duration() >= config_->timestamp_) {
        auto w = config_->width_white_key_;
        auto h = config_->height_white_key_;
        auto x = config_->key_map_[key] - w * 0.5f;
        auto y = config_->stage_->height() - config_->height_white_key_;
        drawRectangle(x, y, w, h);
      }
    }
  }

  // draw black keys
  glColor4f(0, 0, 0, 1);
  for (int i = 0; i < NUM_PIANO_KEYS; ++i) {
    if (config_->is_white_key_[i]) {
      continue;
    }
    auto w = config_->width_black_key_;
    auto h = config_->height_black_key_;
    auto x = config_->key_map_[i] - w * 0.5f;
    auto y = config_->stage_->height() - config_->height_white_key_;
    drawRectangle(x, y, w, h);
  }

  // draw events of white keys
  glColor4f(config_->keyboard_color_.redF(), config_->keyboard_color_.greenF(),
            config_->keyboard_color_.blueF(), 1);
  if (config_->notes_) {
    for (const auto &note : config_->notes_->notes()) {
      if (note.note() < MIN_NOTE_ON_KEYBOARD ||
          note.note() > MAX_NOTE_ON_KEYBOARD) {
        continue;
      }

      const auto key = note.note() - MIN_NOTE_ON_KEYBOARD;
      if (config_->is_white_key_[key]) {
        continue;
      }

      if (note.timestamp() <= config_->timestamp_ &&
          note.timestamp() + note.duration() >= config_->timestamp_) {
        auto w = config_->width_black_key_;
        auto h = config_->height_black_key_;
        auto x = config_->key_map_[key] - w * 0.5f;
        auto y = config_->stage_->height() - config_->height_white_key_;
        drawRectangle(x, y, w, h);
      }
    }
  }

  glEnable(GL_BLEND);
}

}  // namespace midi
