#pragma once

#include "openglwidget.h"

#include <QElapsedTimer>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QWindow>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>

class Window : public OpenGLWidget
{
  Q_OBJECT

public:
  Window();
  ~Window();

  void onInit() final;
  void onRender() final;
  void onResize(size_t W, size_t H) final;

  void keyPressEvent(QKeyEvent *Event) final;
  void mousePressEvent(QMouseEvent *Event) final;
  void mouseMoveEvent(QMouseEvent *Event) final;
  void wheelEvent(QWheelEvent *Event) final;

private:
  QElapsedTimer timer_;
  float hue_ = 0;

  QPointF LastMousePos {NAN, NAN};

  QVector2D Resolution {NAN, NAN};
  QVector2D AspectRatio {NAN, NAN};

  float FractalZoomCounter {0};
  float FractalZoomFactor {std::log(1.05f)};
  float FractalZoom {1.f};
  QVector2D FractalCenter {0.f, 0.f};
  QVector2D FractalExtent;
  bool FractalInfoUpdated {true};

  QOpenGLBuffer vbo_ {QOpenGLBuffer::Type::VertexBuffer};
  QOpenGLBuffer ibo_ {QOpenGLBuffer::Type::IndexBuffer};
  QOpenGLVertexArrayObject vao_;

  std::unique_ptr<QOpenGLTexture> texture_;
  std::unique_ptr<QOpenGLShaderProgram> program_;
};
