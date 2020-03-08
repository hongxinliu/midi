#include "midi/show/stage.h"
#include <QTimer>
#include <chrono>
#include "midi/show/actors/actor_background.h"
#include "midi/show/actors/actor_flash.h"
#include "midi/show/actors/actor_keyboard.h"
#include "midi/show/actors/actor_note.h"
#include "midi/show/actors/actor_particle.h"
#include "midi/show/config.h"

namespace midi {

Stage::Stage() { config_ = Singleton<Config>::get(); }

int Stage::heightWithoutKeyboard() const {
  return (config_->show_keyboard_ ? this->height() - config_->height_white_key_
                                  : this->height());
}

bool Stage::loadBackground(const std::string &path) {
  return dynamic_cast<ActorBackground *>(actors_.front())->loadBackground(path);
}

void Stage::initializeGL() {
  initializeOpenGLFunctions();

  glClearColor(0, 0, 0, 1);

  actors_.push_back(
      new ActorBackground());  // background should always be the first one
  actors_.push_back(new ActorParticle());
  actors_.push_back(new ActorNote());
  actors_.push_back(new ActorKeyboard());
  actors_.push_back(new ActorFlash());

  for (auto &actor : actors_) {
    actor->initialize();
  }

  auto timer = new QTimer();
  QObject::connect(timer, &QTimer::timeout, [this]() {
    static const int64_t us_per_frame = 1000000.0 / config_->fps_;
    static auto last = std::chrono::system_clock::now();
    auto current = std::chrono::system_clock::now();
    if (std::chrono::duration_cast<std::chrono::microseconds>(current - last)
            .count() >= us_per_frame) {
      this->update();
      last = current;
    }
  });
  timer->start(1);
}

void Stage::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_LINE_SMOOTH);

  int viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  glPushMatrix();
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  for (auto &actor : actors_) {
    actor->perform();
  }
}

void Stage::resizeGL(int w, int h) { setupKeyboard(); }

void Stage::setupKeyboard() {
  const float width_white_key_with_gap =
      1.0 * this->width() / NUM_PIANO_WHITE_KEYS;
  const float width_group = width_white_key_with_gap * NUM_WHITE_KEYS_PER_GROUP;

  config_->width_white_key_ = width_white_key_with_gap * 0.92;
  config_->width_black_key_ = config_->width_white_key_ * 0.75;
  config_->height_white_key_ = this->height() * 0.16;
  config_->height_black_key_ = config_->height_white_key_ * 0.65;

  config_->key_map_[0] = width_white_key_with_gap * 0.5f;
  config_->key_map_[2] = width_white_key_with_gap * 1.5f;
  config_->key_map_[3] = width_white_key_with_gap * 2.5f;
  config_->key_map_[5] = width_white_key_with_gap * 3.5f;
  config_->key_map_[7] = width_white_key_with_gap * 4.5f;
  config_->key_map_[8] = width_white_key_with_gap * 5.5f;
  config_->key_map_[10] = width_white_key_with_gap * 6.5f;
  config_->key_map_[12] = width_white_key_with_gap * 7.5f;
  config_->key_map_[1] = (config_->key_map_[0] + config_->key_map_[2]) * 0.5;
  config_->key_map_[4] = (config_->key_map_[3] + config_->key_map_[5]) * 0.5;
  config_->key_map_[6] = (config_->key_map_[5] + config_->key_map_[7]) * 0.5;
  config_->key_map_[9] = (config_->key_map_[8] + config_->key_map_[10]) * 0.5;
  config_->key_map_[11] = (config_->key_map_[10] + config_->key_map_[12]) * 0.5;
  for (int i = 13; i < NUM_PIANO_KEYS; ++i) {
    config_->key_map_[i] = config_->key_map_[i - 12] + width_group;
  }
}

}  // namespace midi
