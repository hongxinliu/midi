#pragma once

#include <QPushButton>
#include <functional>

namespace midi {

class ColorButton : public QPushButton {
 public:
  ColorButton(const QColor &color,
              const std::function<void(const QColor &)> &callback,
              QWidget *parent = nullptr);

 protected:
  void setColor(const QColor &color);
};

}  // namespace midi
