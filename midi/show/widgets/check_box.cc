#include "midi/show/widgets/check_box.h"

namespace midi {

CheckBox::CheckBox(const QString &text, const bool is_checked,
                   const std::function<void(const bool)> &callback)
    : QCheckBox(text) {
  this->setChecked(is_checked);
  QObject::connect(this, &QCheckBox::stateChanged,
                   [=](const bool is_checked) { callback(is_checked); });
}

}  // namespace midi
