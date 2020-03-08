#pragma once

#include <QPushButton>
#include <functional>

namespace midi {

class PushButton : public QPushButton {
 public:
  PushButton(const std::function<void()> &callback);
  PushButton(const QString &text, const std::function<void()> &callback);
  PushButton(const QIcon &icon, const QString &text,
             const std::function<void()> &callback);
};

}  // namespace midi
