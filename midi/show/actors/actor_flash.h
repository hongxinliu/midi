#pragma once

#include "midi/show/actors/actor.h"

namespace midi {

class ActorFlash : public Actor {
 public:
  void initialize();

  void perform() override;

 protected:
  std::vector<std::shared_ptr<QOpenGLTexture>> textures_;
};

}  // namespace midi
