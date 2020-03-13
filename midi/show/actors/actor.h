#pragma once

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <memory>

namespace midi {

class Config;

class Actor : protected QOpenGLFunctions {
 public:
  Actor();

  virtual ~Actor() {}

 public:
  virtual void initialize();

  virtual void perform() = 0;

 protected:
  void drawPoints(const size_t num_points, const float *const px,
                  const float *const py);

  void drawEllipse(const float x, const float y, const float radius_x,
                   const float radius_y, const bool fill = true,
                   const float step = M_PI / 180.f);

  void drawRectangle(const float x, const float y, const float w, const float h,
                     const bool fill = true);

  void drawRoundedRectangle(const float x, const float y, const float w,
                            const float h, const float radius,
                            const bool fill = true,
                            const float step = M_PI / 180.f);

  void drawTexture(const std::shared_ptr<QOpenGLTexture> &texture,
                   const float x, const float y, const float w, const float h);

  std::shared_ptr<QOpenGLTexture> loadTexture(const std::string &path) const;

 protected:
  Config *config_;
};

}  // namespace midi
