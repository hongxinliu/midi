#include "midi/show/actors/actor_background.h"
#include "midi/show/config.h"
#include "midi/show/stage.h"

namespace midi {

void ActorBackground::perform() {
  if (!texture_) {
    return;
  }

  glColor4f(1, 1, 1, config_->background_alpha_);
  drawTexture(texture_, 0, 0, config_->stage_->width(),
              config_->stage_->heightWithoutKeyboard());
}

bool ActorBackground::loadBackground(const std::string &path) {
  texture_ = loadTexture(path);
  return true;
}

}  // namespace midi
