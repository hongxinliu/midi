#pragma once

#include <QWidget>

namespace midi {

class Config;

class Director : public QWidget {
 public:
  Director();

 protected:
  Config *config_;
};

}  // namespace midi
