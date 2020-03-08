#pragma once

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <list>

namespace midi {

class Config;
class Actor;

class Stage : public QOpenGLWidget, protected QOpenGLFunctions {
 public:
  Stage();

 public:
  int heightWithoutKeyboard() const;

  bool loadBackground(const std::string &path);

 protected:
  void initializeGL() override;

  void paintGL() override;

  void resizeGL(int w, int h) override;

 protected:
  void setupKeyboard();

 protected:
  std::list<Actor *> actors_;
  Config *config_;
};

}  // namespace midi
