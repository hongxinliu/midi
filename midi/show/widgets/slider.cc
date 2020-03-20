#include "midi/show/widgets/slider.h"
#include <QLabel>
#include <QLayout>
#include <cmath>
#include <iomanip>
#include <sstream>

namespace midi {

Slider::Slider(const QString &text, const int precision, const double min,
               const double max, const double val,
               const std::function<void(const double)> &callback) {
  auto label_name = new QLabel(text);
  auto label_value = new QLabel();
  auto slider = new QSlider(Qt::Horizontal);

  std::stringstream ss;
  ss << std::setprecision(precision) << std::fixed << val;
  label_value->setText(QString::fromStdString(ss.str()));

  slider->setMinimum(min * std::pow(10, precision));
  slider->setMaximum(max * std::pow(10, precision));
  slider->setValue(val * std::pow(10, precision));

  QObject::connect(slider, &QSlider::valueChanged, [=](const int val) {
    double v = val * std::pow(10, -precision);
    std::stringstream ss;
    ss << std::setprecision(precision) << std::fixed << v;
    label_value->setText(QString::fromStdString(ss.str()));
    callback(v);
  });

  auto hbox = new QHBoxLayout();
  hbox->addWidget(label_name);
  hbox->addWidget(slider);
  hbox->addWidget(label_value);
  this->setLayout(hbox);
}

}  // namespace midi
