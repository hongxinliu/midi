#include "midi/show/actors/actor_flash.h"
#include <QDir>
#include <boost/filesystem.hpp>
#include <random>
#include "midi/proto/note.pb.h"
#include "midi/show/config.h"
#include "midi/show/stage.h"
#include "midi/show/theater.h"

namespace midi {

void ActorFlash::initialize() {
  Actor::initialize();

  // load textures
  const QString flash_dir =
      QString::fromStdString(boost::filesystem::path(config_->binary_path_)
                                 .append("resources/midi/show/flash")
                                 .string());
  QDir dir(flash_dir);
  auto paths = dir.entryList(QStringList() << "*.jpg"
                                           << "*.png"
                                           << "*.bmp"
                                           << "*.gif",
                             QDir::Files);
  for (const auto &path : paths) {
    textures_.push_back(loadTexture((flash_dir + "/" + path).toStdString()));
  }
}

void ActorFlash::perform() {
  if (!config_->notes_ || textures_.empty()) {
    return;
  }

  glColor4f(1, 1, 1, 1);

  for (const auto &note : config_->notes_->notes()) {
    if (note.note() < MIN_NOTE_ON_KEYBOARD ||
        note.note() > MAX_NOTE_ON_KEYBOARD) {
      continue;
    }

    if (note.timestamp() <= config_->timestamp_ &&
        note.timestamp() + note.duration() > config_->timestamp_) {
      const auto key = note.note() - MIN_NOTE_ON_KEYBOARD;

      static std::default_random_engine e;
      static std::uniform_int_distribution<int> d(0, textures_.size() - 1);

      const auto size = config_->width_white_key_ * 1.5f;
      auto x = config_->key_map_[key] - size * 0.5f;
      auto y = config_->stage_->heightWithoutKeyboard() - size * 0.65f;
      drawTexture(textures_[d(e)], x, y, size, size);
    }
  }
}

}  // namespace midi
