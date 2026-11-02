#include "window.h"

#include <array>
#include <QLabel>

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
  texture_.reset();
  program_.reset();
}

void Window::onInit()
{
  // Configure shaders
  program_ = std::make_unique<QOpenGLShaderProgram>(this);
  program_->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/template.vs");
  program_->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/template.fs");
  if (!program_->link()) {
      qDebug() << "Ошибка линковки:" << program_->log();
  }
  program_->bind();

  // Configure textures
  texture_ = std::make_unique<QOpenGLTexture>(QImage(":/textures/cat.jpg"));
  texture_->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
  texture_->setWrapMode(QOpenGLTexture::WrapMode::Repeat);
  program_->setUniformValue("tex_2d", 0);

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

  // Start timer for animation (if needed)
  timer_.start();

  // Еnable depth test and face culling
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
}

void Window::onRender()
{
  glClear(GL_COLOR_BUFFER_BIT);

  // Bind shader program
  program_->bind();

  if (animated_ && сolorAnimated_)
  {
    const float dt = timer_.restart() / 1000.0f;
    hue_ = std::fmod(hue_ + dt * (60.0f / 360.0f), 1.0f);
    program_->setUniformValue(2, QColor::fromHsvF(hue_, 1.0, 1.0));
  }

  // Add textures
  glActiveTexture(GL_TEXTURE0);
  texture_->bind();

  // Bind VAO
  vao_.bind();

  // Draw
  glDrawElements(GL_TRIANGLES, std::size(Indices), GL_UNSIGNED_INT, nullptr);

  // Release all
  vao_.release();
  program_->release();
  texture_->release();

  if (animated_)
    update();
}

void Window::onResize(const size_t width, const size_t height)
{
  // Configure viewport
  glViewport(0, 0, static_cast<GLint>(width), static_cast<GLint>(height));
}
