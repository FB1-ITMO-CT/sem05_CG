#pragma once

#include "openglwidget.h"

#include <QElapsedTimer>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QWindow>

class Window : public OpenGLWidget
{
    Q_OBJECT

public:
    Window();
    ~Window();

    void onInit() override;
    void onRender() override;
    void onResize(size_t width, size_t height) override;

private:
  QElapsedTimer timer_;
  float hue_ = 0;

  QOpenGLBuffer vbo_{QOpenGLBuffer::Type::VertexBuffer};
  QOpenGLBuffer ibo_{QOpenGLBuffer::Type::IndexBuffer};
  QOpenGLVertexArrayObject vao_;

  std::unique_ptr<QOpenGLTexture> texture_;
  std::unique_ptr<QOpenGLShaderProgram> program_;

  bool animated_ = true;
  bool сolorAnimated_ = animated_;
};
