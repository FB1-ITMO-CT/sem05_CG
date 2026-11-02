#pragma once

#include "openglwidget.h"
#include "sliders.hpp"

#include <QElapsedTimer>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QWindow>
#include <QSlider>
#include <QLabel>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QHBoxLayout>

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
  std::unique_ptr<QLabel> FPSLabel;

  static constexpr slider_config SlidersConfig[4]
  {
    {"Iterations:", 16, 16384, 1024, 16},
    {"A:", -64, 64,  1, 1},
    {"B:", -64, 64,  3, 1},
    {"C:", -64, 64, 30, 1},
  };
  slider_panel Sliders {this, SlidersConfig};

  QPointF LastMousePos {NAN, NAN};

  QVector2D Resolution {NAN, NAN};
  QVector2D TexelSize {NAN, NAN};
  QVector2D AspectRatio {NAN, NAN};

  float FractalZoomCounter {0};
  float FractalZoomFactor {std::log(1.05f)};
  float FractalZoom {1.f};
  QVector2D FractalCenter {0.f, 0.f};
  QVector2D FractalExtent;

  QOpenGLBuffer vbo_ {QOpenGLBuffer::Type::VertexBuffer};
  QOpenGLBuffer ibo_ {QOpenGLBuffer::Type::IndexBuffer};
  QOpenGLVertexArrayObject vao_;

  std::unique_ptr<QOpenGLShaderProgram> program_;
};
