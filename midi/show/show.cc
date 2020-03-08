#include <QApplication>
#include "midi/show/theater.h"

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  midi::Theater theater;
  theater.show();
  return app.exec();
}
