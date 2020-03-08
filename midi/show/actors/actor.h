#pragma once

#include <QOpenGLFunctions>
#include <QOpenGLTexture>
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

  void drawRectangle(const float x, const float y, const float w,
                     const float h);

  void drawTexture(const std::shared_ptr<QOpenGLTexture> &texture,
                   const float x, const float y, const float w, const float h);

  std::shared_ptr<QOpenGLTexture> loadTexture(const std::string &path) const;

 protected:
  Config *config_;
};

}  // namespace midi
