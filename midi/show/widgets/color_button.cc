#include "midi/show/widgets/color_button.h"
#include <QColorDialog>

namespace midi {

ColorButton::ColorButton(const QColor &color,
                         const std::function<void(const QColor &)> &callback,
                         QWidget *parent) {
  setColor(color);
  QObject::connect(this, &QPushButton::clicked, [=]() {
    auto color = QColorDialog::getColor(Qt::white, parent);
    if (color.isValid()) {
      this->setColor(color);
      callback(color);
    }
  });
}

void ColorButton::setColor(const QColor &color) {
  auto pal = this->palette();
  pal.setColor(QPalette::Button, color);
  this->setAutoFillBackground(true);
  this->setPalette(pal);
  this->update();
}

}  // namespace midi
