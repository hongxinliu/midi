#include "midi/show/actors/actor_note.h"
#include "midi/proto/note.pb.h"
#include "midi/show/config.h"
#include "midi/show/stage.h"
#include "midi/show/theater.h"

namespace midi {

void ActorNote::perform() {
  if (!config_->notes_) {
    return;
  }

  auto from = config_->timestamp_;
  auto to = from + config_->time_scope_;

  glColor4f(config_->note_color_.redF(), config_->note_color_.greenF(),
            config_->note_color_.blueF(), config_->note_alpha_);

  for (const auto &note : config_->notes_->notes()) {
    if (note.note() < MIN_NOTE_ON_KEYBOARD ||
        note.note() > MAX_NOTE_ON_KEYBOARD) {
      continue;
    }

    if (note.timestamp() > to || note.timestamp() + note.duration() < from) {
      continue;
    }

    const auto key = note.note() - MIN_NOTE_ON_KEYBOARD;

    const float w = (config_->is_white_key_[key] ? config_->width_white_key_
                                                 : config_->width_black_key_);
    const float h = 1.0f * note.duration() / config_->time_scope_ *
                    config_->stage_->heightWithoutKeyboard();
    const float x = config_->key_map_[key] - w * 0.5f;
    const float y = 1.0f * config_->stage_->heightWithoutKeyboard() *
                    (note.timestamp() + note.duration() - to) / (from - to);
    drawRectangle(x, y, w, h);
  }
}

}  // namespace midi
