#include "midi/show/actors/actor_particle.h"
#include <random>
#include "midi/proto/note.pb.h"
#include "midi/show/config.h"
#include "midi/show/stage.h"
#include "midi/show/theater.h"

namespace midi {

ActorParticle::ActorParticle() : gravity_(0, 0.0002) {}

void ActorParticle::perform() {
  if (!config_->notes_) {
    return;
  }

  static std::default_random_engine e;
  static std::uniform_int_distribution<int> d_x(-3, 3);
  static std::uniform_real_distribution<double> d_vx(-0.02, 0.02);
  static std::uniform_real_distribution<double> d_vy(-0.1, 0.1);

  // new particles
  for (const auto &note : config_->notes_->notes()) {
    if (note.note() < MIN_NOTE_ON_KEYBOARD ||
        note.note() > MAX_NOTE_ON_KEYBOARD) {
      continue;
    }

    if (note.timestamp() <= config_->timestamp_ &&
        note.timestamp() + note.duration() > config_->timestamp_) {
      const auto key = note.note() - MIN_NOTE_ON_KEYBOARD;
      double x = config_->key_map_[key] + d_x(e);
      double y = config_->stage_->heightWithoutKeyboard();

      auto clamp = [](double v, double min, double max) {
        return (v < min ? min : (v > max ? max : v));
      };

      double vy_duration = -clamp((note.duration() / 100000) * 0.01, 0.05, 0.2);
      double vy_velocity = -clamp((note.velocity() / 10) * 0.03, 0.05, 0.2);
      double vy = vy_duration + vy_velocity + d_vy(e);

      std::shared_ptr<Particle> particle(
          new Particle(gravity_, Vec2d(x, y), Vec2d(d_vx(e), vy),
                       config_->particle_lifetime_));
      particles_.push_front(particle);
    }
  }

  // step particles
  for (auto it = particles_.begin(); it != particles_.end();) {
    if (!it->get()->step(1000.0 / config_->fps_) ||
        it->get()->pos_.y > config_->stage_->heightWithoutKeyboard()) {
      it = particles_.erase(it);
    } else {
      ++it;
    }
  }

  // drop some particles
  if (particles_.size() > config_->particle_limit_) {
    particles_.resize(config_->particle_limit_);
  }

  // draw particles
  int num_points = particles_.size();
  std::vector<float> x, y;
  x.reserve(num_points);
  y.reserve(num_points);
  for (const auto &p : particles_) {
    x.push_back(p->pos_.x);
    y.push_back(p->pos_.y);
  }
  glColor4f(config_->particle_color_.redF(), config_->particle_color_.greenF(),
            config_->particle_color_.blueF(), config_->particle_alpha_);
  glPointSize(config_->particle_size_);
  drawPoints(num_points, x.data(), y.data());
}

}  // namespace midi
