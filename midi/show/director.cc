#include "midi/show/director.h"
#include <QColorDialog>
#include <QFileDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSlider>
#include "midi/show/config.h"
#include "midi/show/stage.h"
#include "midi/show/theater.h"
#include "midi/show/widgets/check_box.h"
#include "midi/show/widgets/color_button.h"
#include "midi/show/widgets/push_button.h"
#include "midi/show/widgets/slider.h"

namespace midi {

Director::Director() {
  config_ = Singleton<Config>::get();

  // help info
  auto label_help = new QLabel("Press F12 to show/hide this widget");
  QFont font;
  font.setPixelSize(12);
  label_help->setFont(font);
  label_help->setAlignment(Qt::AlignCenter);

  // file
  auto label_file = new QLabel("File");
  label_file->setAlignment(Qt::AlignCenter);
  auto bt_open = new PushButton("Open MIDI File(O)",
                                [=]() { config_->theater_->loadFile(); });
  auto bt_play_pause =
      new PushButton("Play/Pause(P)", [=]() { config_->theater_->toggle(); });
  auto bt_stop =
      new PushButton("Stop(S)", [=]() { config_->theater_->stop(); });
  auto bt_restart =
      new PushButton("Restart(R)", [=]() { config_->theater_->restart(); });
  auto layout_file = new QVBoxLayout();
  auto layout_file_bts = new QHBoxLayout();
  layout_file_bts->addWidget(bt_play_pause);
  layout_file_bts->addWidget(bt_stop);
  layout_file_bts->addWidget(bt_restart);
  layout_file->addWidget(label_file);
  layout_file->addWidget(bt_open);
  layout_file->addLayout(layout_file_bts);
  auto gb_file = new QGroupBox();
  gb_file->setLayout(layout_file);

  // background
  auto label_background = new QLabel("Background");
  label_background->setAlignment(Qt::AlignCenter);
  auto bt_load_background = new PushButton("Load Background Image(K)", [=]() {
    auto path = QFileDialog::getOpenFileName(this, "Open Image File");
    if (!path.isEmpty()) {
      config_->stage_->loadBackground(path.toStdString());
    }
  });
  auto slider_background_alpha =
      new Slider("Alpha", 2, 0, 1, config_->background_alpha_,
                 [=](const double val) { config_->background_alpha_ = val; });
  auto layout_background = new QVBoxLayout();
  layout_background->addWidget(label_background);
  layout_background->addWidget(bt_load_background);
  layout_background->addWidget(slider_background_alpha);
  auto gb_background = new QGroupBox();
  gb_background->setLayout(layout_background);

  // particle
  auto label_particle = new QLabel("Particles");
  label_particle->setAlignment(Qt::AlignCenter);
  auto cb_particle_trail = new CheckBox(
      "Show Trail", config_->particle_trail_,
      [=](const bool is_checked) { config_->particle_trail_ = is_checked; });
  auto bt_particle_color = new ColorButton(
      config_->particle_color_,
      [=](const QColor &color) { config_->particle_color_ = color; },
      config_->theater_);
  auto slider_particle_alpha =
      new Slider("Alpha", 2, 0, 1, config_->particle_alpha_,
                 [=](const double val) { config_->particle_alpha_ = val; });
  auto layout_particle = new QVBoxLayout();
  layout_particle->addWidget(label_particle);
  layout_particle->addWidget(cb_particle_trail);
  layout_particle->addWidget(bt_particle_color);
  layout_particle->addWidget(slider_particle_alpha);
  auto gb_particle = new QGroupBox();
  gb_particle->setLayout(layout_particle);

  // note
  auto label_note = new QLabel("Notes");
  label_note->setAlignment(Qt::AlignCenter);
  auto bt_note_color = new ColorButton(
      config_->note_color_,
      [=](const QColor &color) { config_->note_color_ = color; },
      config_->theater_);
  auto slider_note_alpha =
      new Slider("Alpha", 2, 0, 1, config_->note_alpha_,
                 [=](const double val) { config_->note_alpha_ = val; });
  auto layout_note = new QVBoxLayout();
  layout_note->addWidget(label_note);
  layout_note->addWidget(bt_note_color);
  layout_note->addWidget(slider_note_alpha);
  auto gb_note = new QGroupBox();
  gb_note->setLayout(layout_note);

  // keyboard
  auto label_keyboard = new QLabel("Keyboard");
  label_keyboard->setAlignment(Qt::AlignCenter);
  auto cb_show_keyboard = new CheckBox(
      "Show Keyboard", config_->show_keyboard_,
      [=](const bool is_checked) { config_->show_keyboard_ = is_checked; });
  auto bt_keyboard_color = new ColorButton(
      config_->keyboard_color_,
      [=](const QColor &color) { config_->keyboard_color_ = color; },
      config_->theater_);
  auto layout_keyboard = new QVBoxLayout();
  layout_keyboard->addWidget(label_keyboard);
  layout_keyboard->addWidget(cb_show_keyboard);
  layout_keyboard->addWidget(bt_keyboard_color);
  auto gb_keyboard = new QGroupBox();
  gb_keyboard->setLayout(layout_keyboard);

  // layout
  auto layout = new QVBoxLayout();
  layout->addWidget(label_help);
  layout->addStretch();
  layout->addWidget(gb_file);
  layout->addStretch();
  layout->addWidget(gb_background);
  layout->addStretch();
  layout->addWidget(gb_particle);
  layout->addStretch();
  layout->addWidget(gb_note);
  layout->addStretch();
  layout->addWidget(gb_keyboard);
  layout->addStretch();
  layout->setAlignment(Qt::AlignTop);
  layout->setMargin(5);
  layout->setSpacing(0);
  this->setLayout(layout);
}

}  // namespace midi
