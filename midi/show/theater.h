#pragma once

#include <QKeyEvent>
#include <QTimer>
#include <QWidget>

namespace midi {

class Config;

class Theater : public QWidget {
 public:
  Theater();

 public:
  void loadFile();

  void play();

  void pause();

  void stop();

  void restart();

  void toggle();

 protected:
  void keyPressEvent(QKeyEvent *event) override;

 protected:
  Config *config_;
  QTimer *timer_;
  int64_t utime_play_;
};

}  // namespace midi
