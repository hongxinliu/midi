#pragma once

#include <QSlider>
#include <functional>

namespace midi {

class Slider : public QWidget {
 public:
  Slider(const QString &text, const int precision, const double min,
         const double max, const double val,
         const std::function<void(const double)> &callback);
};

}  // namespace midi
