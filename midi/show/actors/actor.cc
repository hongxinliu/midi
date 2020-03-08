#include "midi/show/actors/actor.h"
#include <QImage>
#include "midi/show/config.h"

namespace midi {

Actor::Actor() { config_ = Singleton<Config>::get(); }

void Actor::initialize() { initializeOpenGLFunctions(); }

void Actor::drawPoints(const size_t num_points, const float *const px,
                       const float *const py) {
  glBegin(GL_POINTS);
  for (size_t i = 0; i < num_points; ++i) {
    glVertex2f(px[i], py[i]);
  }
  glEnd();
}

void Actor::drawRectangle(const float x, const float y, const float w,
                          const float h) {
  glBegin(GL_QUADS);
  glVertex2f(x, y);
  glVertex2f(x + w, y);
  glVertex2f(x + w, y + h);
  glVertex2f(x, y + h);
  glEnd();
}

void Actor::drawTexture(const std::shared_ptr<QOpenGLTexture> &texture,
                        const float x, const float y, const float w,
                        const float h) {
  glEnable(GL_TEXTURE_2D);
  texture->bind();

  glBegin(GL_QUADS);
  glTexCoord2f(0, 0);
  glVertex2f(x, y);
  glTexCoord2f(1, 0);
  glVertex2f(x + w, y);
  glTexCoord2f(1, 1);
  glVertex2f(x + w, y + h);
  glTexCoord2f(0, 1);
  glVertex2f(x, y + h);
  glEnd();

  texture->release();
  glDisable(GL_TEXTURE_2D);
}

std::shared_ptr<QOpenGLTexture> Actor::loadTexture(
    const std::string &path) const {
  QImage qimg(QString::fromStdString(path));
  return std::make_shared<QOpenGLTexture>(qimg);
}

}  // namespace midi
