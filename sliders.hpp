#pragma once

#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFontMetrics>
#include <QOpenGLWidget>
#include <QEvent>

#include <algorithm>
#include <memory>
#include <span>
#include <vector>

struct slider_config
{
  const char *title;
  int minimum;
  int maximum;
  int initial;
  int step;
};

class slider_panel final : public QWidget
{
public:
  slider_panel(QOpenGLWidget *glWidget, std::span<const slider_config> configs) :
    QWidget {glWidget}
  {
    setAutoFillBackground(true);

    Outer.reset(new QVBoxLayout(this));
    Outer->setContentsMargins(8, 8, 8, 8);
    Outer->setSpacing(4);

    QFontMetrics fm(font());
    int titleWidth = 0;
    for (const auto &cfg : configs)
      titleWidth = std::max(titleWidth, fm.horizontalAdvance(QString::fromUtf8(cfg.title)));
    titleWidth += 8;

    Entries.reserve(configs.size());
    for (const auto &cfg : configs)
    {
      auto e {std::make_unique<Entry>()};

      e->Row = std::make_unique<QWidget>(this);
      auto *rowLayout {new QHBoxLayout(e->Row.get())};
      rowLayout->setContentsMargins(0, 0, 0, 0);
      rowLayout->setSpacing(6);

      e->Title = std::make_unique<QLabel>(QString::fromUtf8(cfg.title), e->Row.get());
      e->Title->setMinimumWidth(titleWidth);

      e->Slider = std::make_unique<QSlider>(Qt::Horizontal, e->Row.get());
      e->Slider->setRange(cfg.minimum, cfg.maximum);
      e->Slider->setSingleStep(cfg.step);
      e->Slider->setPageStep(cfg.step * 10);
      e->Slider->setValue(cfg.initial);

      e->Value = std::make_unique<QLabel>(QString::number(cfg.initial), e->Row.get());
      e->Value->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
      e->Value->setMinimumWidth(fm.horizontalAdvance(QString::number(cfg.maximum)) + 8);

      rowLayout->addWidget(e->Title.get());
      rowLayout->addWidget(e->Slider.get(), 1);
      rowLayout->addWidget(e->Value.get());

      QSlider *Slider = e->Slider.get();
      QLabel  *Label  = e->Value.get();
      connect(Slider, &QSlider::valueChanged, Label, [Label](int v)
      {
        Label->setText(QString::number(v));
      });

      connect(Slider, &QSlider::valueChanged, glWidget, QOverload<>::of(&QWidget::update));

      Outer->addWidget(e->Row.get());
      Entries.push_back(std::move(e));
    }

    syncGeometry();
    glWidget->installEventFilter(this);
    raise();
  }

  int Value(std::size_t index) const
  {
    return Entries.at(index)->Slider->value();
  }

  std::size_t count() const noexcept
  {
    return Entries.size();
  }

protected:
  bool eventFilter(QObject *watched, QEvent *event) override
  {
    if (watched == parentWidget() && event->type() == QEvent::Resize)
      syncGeometry();
    return QWidget::eventFilter(watched, event);
  }

private:
  struct Entry
  {
    std::unique_ptr<QWidget> Row;
    std::unique_ptr<QLabel>  Title;
    std::unique_ptr<QSlider> Slider;
    std::unique_ptr<QLabel>  Value;
  };

  void syncGeometry()
  {
    if (auto *p = parentWidget())
      setGeometry(0, 0, p->width(), sizeHint().height());
  }

  std::vector<std::unique_ptr<Entry>> Entries;

  std::unique_ptr<QVBoxLayout> Outer;
};
