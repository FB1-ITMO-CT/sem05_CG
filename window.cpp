#include "window.h"

#include <array>
#include <QLabel>
#include <qevent.h>

struct vertex
{
  GLfloat X, Y;
};

constexpr vertex Vertices[] {
  {-1.f,  1.f},
  {-1.f, -1.f},
  { 1.f,  1.f},
  { 1.f, -1.f},
};

constexpr GLuint Indices[] {0, 1, 2, 2, 1, 3};

Window::Window()
{
  // Add label for FPS
  auto* fpsLabel = new QLabel("FPS: 0", this);
  fpsLabel->setStyleSheet("QLabel { color: white; }");
  fpsLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
  fpsLabel->adjustSize();
  fpsLabel->move(0, 0);
  fpsLabel->raise();

  connect(this, &Window::fpsUpdated, [fpsLabel](double fps) {
    fpsLabel->setText(QString("FPS: %1").arg(fps, 0, 'f', 0));
    fpsLabel->adjustSize();
  });
}

Window::~Window()
{
  program_.reset();
}

void Window::onInit()
{
  setMouseTracking(true);

  // Configure shaders
  program_ = std::make_unique<QOpenGLShaderProgram>(this);
  program_->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/template.vs");
  program_->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/template.fs");
  if (!program_->link())
    qDebug() << "Linking error:" << program_->log();
  program_->bind();

  // Create VAO object
  vao_.create();
  vao_.bind();

  // Create VBO
  vbo_.create();
  vbo_.bind();
  vbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  vbo_.allocate(std::data(Vertices), static_cast<int>(std::size(Vertices) * sizeof(Vertices[0])));

  // Create IBO
  ibo_.create();
  ibo_.bind();
  ibo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  ibo_.allocate(std::data(Indices), static_cast<int>(std::size(Indices) * sizeof(Indices[0])));

  // Bind attributes
  program_->enableAttributeArray(0);
  program_->setAttributeBuffer(0, GL_FLOAT, 0, 2, static_cast<int>(sizeof(Vertices[0])));

  // Release all
  program_->release();

  vao_.release();
  vbo_.release();
  ibo_.release();

  // Еnable depth test and face culling
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
}

void Window::onRender()
{
  glClear(GL_COLOR_BUFFER_BIT);

  // Bind shader program
  program_->bind();

  program_->setUniformValue(0, FractalCenter.x(), FractalCenter.y(), FractalExtent.x(), FractalExtent.y());

  program_->setUniformValue(1, 1.f, 3.0f, 0.f);

  TexelSize = FractalExtent * 2 / Resolution;
  program_->setUniformValue(2, TexelSize.x(), TexelSize.y());

  // Bind VAO
  vao_.bind();

  // Draw
  glDrawElements(GL_TRIANGLES, std::size(Indices), GL_UNSIGNED_INT, nullptr);

  // Release all
  vao_.release();
  program_->release();

  update();
}

void Window::onResize(const size_t W, const size_t H)
{
  Resolution = {static_cast<float>(W), static_cast<float>(H)};

  AspectRatio = Resolution / std::min(Resolution.x(), Resolution.y());
  FractalExtent = AspectRatio * FractalZoom;

  glViewport(0, 0, static_cast<GLint>(W), static_cast<GLint>(H));
}

void Window::keyPressEvent(QKeyEvent *Event)
{
  QOpenGLWidget::keyPressEvent(Event);
}

void Window::mousePressEvent(QMouseEvent *Event)
{
  QOpenGLWidget::mousePressEvent(Event);
}

void Window::mouseMoveEvent(QMouseEvent *Event)
{
  if (LastMousePos != LastMousePos) // Evil trickery
    LastMousePos = Event->windowPos();

  if (Event->buttons() & Qt::LeftButton)
  {
    QPointF Delta = Event->windowPos() - LastMousePos;
    QVector2D VDelta {static_cast<float>(-Delta.x()), static_cast<float>(Delta.y())};

    FractalCenter += VDelta / Resolution * 2 * FractalExtent;
    FractalExtent = AspectRatio * FractalZoom;
  }

  LastMousePos = Event->windowPos();

  QOpenGLWidget::mouseMoveEvent(Event);
}

void Window::wheelEvent(QWheelEvent *Event)
{
  float Delta;

  if (QPoint PixelDelta {Event->pixelDelta()}; !PixelDelta.isNull())
    Delta = PixelDelta.y();
  else if (QPoint AngleDelta {Event->angleDelta()}; !AngleDelta.isNull())
    Delta = AngleDelta.y();

  Delta *= -(1.f / 120.f);

  QVector2D Mouse {static_cast<float>(LastMousePos.x()), static_cast<float>(LastMousePos.y())};
  Mouse = Mouse * 2 / Resolution - QVector2D {1.f, 1.f};
  Mouse = Mouse * AspectRatio * FractalExtent;

  const auto BaseMouse {Mouse};
  Mouse /= FractalZoom;

  FractalZoomCounter = std::clamp<float>(FractalZoomCounter + Delta, -1024, 1024);
  FractalZoom = std::exp(static_cast<float>(FractalZoomCounter) * FractalZoomFactor);

  Mouse *= FractalZoom;

  auto MouseDelta {BaseMouse - Mouse};
  MouseDelta.setY(-MouseDelta.y());

  FractalCenter += MouseDelta;

  FractalExtent = AspectRatio * FractalZoom;

  QOpenGLWidget::wheelEvent(Event);
}
