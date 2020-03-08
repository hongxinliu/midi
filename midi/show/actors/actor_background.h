#pragma once

#include "midi/show/actors/actor.h"

namespace midi {

class ActorBackground : public Actor {
 public:
  void perform() override;

  bool loadBackground(const std::string &path);

 protected:
  std::shared_ptr<QOpenGLTexture> texture_;
};

}  // namespace midi
