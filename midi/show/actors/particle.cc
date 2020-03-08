#include "midi/show/actors/particle.h"

namespace midi {

Particle::Particle(const Vec2d &gravity, const Vec2d &pos, const Vec2d &vel,
                   const int64_t lifetime)
    : gravity_(gravity), pos_(pos), vel_(vel), lifetime_(lifetime) {}

bool Particle::step(const int64_t interval) {
  lifetime_ -= interval;
  if (lifetime_ < 0) {
    return false;
  }

  pos_.x += vel_.x * interval;
  pos_.y += vel_.y * interval;

  vel_.x += gravity_.x * interval;
  vel_.y += gravity_.y * interval;

  return true;
}

}  // namespace midi
