#include "midi/show/widgets/push_button.h"

namespace midi {

PushButton::PushButton(const std::function<void()> &callback) {
  QObject::connect(this, &QPushButton::clicked, callback);
}

PushButton::PushButton(const QString &text,
                       const std::function<void()> &callback)
    : QPushButton(text) {
  QObject::connect(this, &QPushButton::clicked, callback);
}

PushButton::PushButton(const QIcon &icon, const QString &text,
                       const std::function<void()> &callback)
    : QPushButton(icon, text) {
  QObject::connect(this, &QPushButton::clicked, callback);
}

}  // namespace midi
