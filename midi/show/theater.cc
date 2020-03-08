#include "midi/show/theater.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QLayout>
#include <QTimer>
#include <boost/filesystem.hpp>
#include <iostream>
#include "midi/io/io.h"
#include "midi/show/binary_path.h"
#include "midi/show/config.h"
#include "midi/show/director.h"
#include "midi/show/stage.h"

namespace midi {

// get timestamp in microseconds
inline int64_t utime() {
  return std::chrono::duration_cast<std::chrono::microseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

Theater::Theater() {
  config_ = Singleton<Config>::get();

  // widgets
  config_->theater_ = this;
  config_->stage_ = new Stage();
  config_->director_ = new Director();

  // layout
  auto layout = new QHBoxLayout();
  layout->addWidget(config_->stage_);
  layout->addWidget(config_->director_);
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->setStretch(0, 4);
  layout->setStretch(1, 1);
  config_->director_->setMaximumWidth(300);
  this->setLayout(layout);

  // set minimum size & move to center
  this->setMinimumSize(QApplication::desktop()->size() * 3 / 4);
  this->move((QApplication::desktop()->width() - this->width()) / 2,
             (QApplication::desktop()->height() - this->height()) / 2);

  // timer to update timestamp
  timer_ = new QTimer();
  QObject::connect(timer_, &QTimer::timeout, [&]() {
    config_->timestamp_ = config_->timestamp_base_ + utime() - utime_play_;
  });
}

void Theater::loadFile() {
  auto path = QFileDialog::getOpenFileName(this, "Open MIDI File", "", "*.mid");
  if (!path.isEmpty()) {
    stop();
    std::cout << "Loading MIDI file: " << path.toStdString() << std::endl;
    auto file = std::make_shared<File>();
    if (load(path.toStdString(), file.get())) {
      std::cout << "Loaded" << std::endl;
      config_->notes_.reset(new Notes());
      if (toNotes(*file, config_->notes_.get())) {
        std::cout << "Paired " << config_->notes_->notes_size() << " notes"
                  << std::endl;
      } else {
        std::cout << "Failed to pair notes" << std::endl;
      }
    } else {
      std::cout << "Failed" << std::endl;
    }
  }
}

void Theater::play() {
  utime_play_ = utime();
  timer_->start(1);
}

void Theater::pause() {
  timer_->stop();
  config_->timestamp_base_ = config_->timestamp_;
}

void Theater::stop() {
  timer_->stop();
  config_->timestamp_base_ = config_->timestamp_reserve_;
  config_->timestamp_ = config_->timestamp_reserve_;
}

void Theater::restart() {
  stop();
  play();
}

void Theater::toggle() {
  if (timer_->isActive()) {
    pause();
  } else {
    play();
  }
}

void Theater::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_F11) {
    if (this->isFullScreen()) {
      this->showNormal();
    } else {
      this->showFullScreen();
    }
  } else if (event->key() == Qt::Key_F12) {
    config_->director_->setVisible(!config_->director_->isVisible());
  } else if (event->key() == Qt::Key_Escape) {
    if (this->isFullScreen()) {
      this->showNormal();
    }
  } else if (event->key() == Qt::Key_O) {
    loadFile();
  } else if (event->key() == Qt::Key_P) {
    toggle();
  } else if (event->key() == Qt::Key_S) {
    stop();
  } else if (event->key() == Qt::Key_R) {
    restart();
  } else if (event->key() == Qt::Key_K) {
    auto path = QFileDialog::getOpenFileName(this, "Open Image File");
    if (!path.isEmpty()) {
      config_->stage_->loadBackground(path.toStdString());
    }
  }
}

}  // namespace midi
