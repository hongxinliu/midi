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

void Actor::drawEllipse(const float x, const float y, const float radius_x,
                        const float radius_y, const bool fill,
                        const float step) {
  if (fill) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (float r = 0; r < M_PI * 2; r += step) {
      glVertex2f(x + radius_x * std::cos(r), y + radius_y * std::sin(r));
    }
    glEnd();
  } else {
    glBegin(GL_LINE_LOOP);
    for (float r = 0; r < M_PI * 2; r += step) {
      glVertex2f(x + radius_x * std::cos(r), y + radius_y * std::sin(r));
    }
    glEnd();
  }
}

void Actor::drawRectangle(const float x, const float y, const float w,
                          const float h, const bool fill) {
  if (fill) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
  } else {
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
  }
}

void Actor::drawRoundedRectangle(const float x, const float y, const float w,
                                 const float h, const float radius,
                                 const bool fill, const float step) {
  const float r = std::min(std::min(w, h) / 2, radius);

  if (fill) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x + w / 2, y + h / 2);
    for (float rad = 0; rad < M_PI_2; rad += step) {
      glVertex2f(x + w - r + r * std::cos(rad), y + h - r + r * std::sin(rad));
    }
    for (float rad = M_PI_2; rad < M_PI; rad += step) {
      glVertex2f(x + r + r * std::cos(rad), y + h - r + r * std::sin(rad));
    }
    for (float rad = M_PI; rad <= M_PI_2 * 3; rad += step) {
      glVertex2f(x + r + r * std::cos(rad), y + r + r * std::sin(rad));
    }
    for (float rad = M_PI_2 * 3; rad <= M_PI * 2; rad += step) {
      glVertex2f(x + w - r + r * std::cos(rad), y + r + r * std::sin(rad));
    }
    glVertex2f(x + w, y + h - r);
    glEnd();
  } else {
    glBegin(GL_LINE_LOOP);
    for (float rad = 0; rad < M_PI_2; rad += step) {
      glVertex2f(x + w - r + r * std::cos(rad), y + h - r + r * std::sin(rad));
    }
    for (float rad = M_PI_2; rad < M_PI; rad += step) {
      glVertex2f(x + r + r * std::cos(rad), y + h - r + r * std::sin(rad));
    }
    for (float rad = M_PI; rad <= M_PI_2 * 3; rad += step) {
      glVertex2f(x + r + r * std::cos(rad), y + r + r * std::sin(rad));
    }
    for (float rad = M_PI_2 * 3; rad <= M_PI * 2; rad += step) {
      glVertex2f(x + w - r + r * std::cos(rad), y + r + r * std::sin(rad));
    }
    glEnd();
  }
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
