#pragma once

#include <QCheckBox>
#include <functional>

namespace midi {

class CheckBox : public QCheckBox {
 public:
  CheckBox(const QString &text, const bool is_checked,
           const std::function<void(const bool)> &callback);
};

}  // namespace midi
