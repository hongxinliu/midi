#pragma once

#include <list>
#include "midi/show/actors/actor.h"
#include "midi/show/actors/particle.h"
#include <QOpenGLFramebufferObject>

namespace midi {

class ActorParticle : public Actor {
 public:
  ActorParticle();

 public:
  void perform() override;

 protected:
  std::list<std::shared_ptr<Particle>> particles_;
  Vec2d gravity_;
  std::shared_ptr<QOpenGLFramebufferObject> background_;
};

}  // namespace midi
