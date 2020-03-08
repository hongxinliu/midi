#pragma once

#include <cstdint>

namespace midi {

struct Vec2d {
  double x;
  double y;

  Vec2d() : x(0.), y(0.) {}
  Vec2d(double x, double y) : x(x), y(y) {}
};

class Particle {
 public:
  Particle(const Vec2d &gravity, const Vec2d &pos, const Vec2d &vel,
           const int64_t lifetime);

 public:
  bool step(const int64_t interval);

 public:
  const Vec2d gravity_;
  Vec2d pos_;
  Vec2d vel_;
  int64_t lifetime_;
};

}  // namespace midi
