#include "midi/show/actors/actor_particle.h"
#include <random>
#include "midi/proto/note.pb.h"
#include "midi/show/config.h"
#include "midi/show/stage.h"
#include "midi/show/theater.h"
#include <opencv2/opencv.hpp>

namespace midi {

ActorParticle::ActorParticle() : gravity_(0, 0.0002) {}

void ActorParticle::perform() {
  if (!config_->notes_) {
    return;
  }

  auto fb = std::make_shared<QOpenGLFramebufferObject>(config_->stage_->width(), config_->stage_->height());
  fb->bind();

  if (config_->particle_trail_ && background_) {
    static auto filter = cv::cuda::createGaussianFilter(CV_8UC3, CV_8UC3, cv::Size(31, 31), 0);
    auto qimg = background_->toImage();
    qimg.convertTo(QImage::Format_BGR888);
    auto m = cv::Mat(qimg.height(), qimg.width(), CV_8UC3, qimg.bits(), qimg.bytesPerLine()).clone();
    cv::cuda::GpuMat gm;
    gm.upload(m);
    if (gm.cols != config_->stage_->width() || gm.rows != config_->stage_->height()) {
      cv::cuda::resize(gm, gm, cv::Size(config_->stage_->width(), config_->stage_->height()));
    }
    filter->apply(gm, gm);
    gm.convertTo(gm, gm.type(), 0.95);
    gm.download(m);
    qimg = QImage(m.data, m.cols, m.rows, m.step, QImage::Format_BGR888);
    auto texture = std::make_shared<QOpenGLTexture>(qimg);
    drawTexture(texture, 0, 0, config_->stage_->width(), config_->stage_->height());
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

  fb->release();
  QOpenGLFramebufferObject::blitFramebuffer(0, fb.get());

  if (config_->particle_trail_) {
    background_ = fb;
  }
}

}  // namespace midi
