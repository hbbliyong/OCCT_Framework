#include "ui/Ribbon/RibbonWidget.h"
#include <QToolBar>
#include <QVBoxLayout>
#include <QAction>

RibbonWidget::RibbonWidget(QWidget* parent)
    : QWidget(parent)
{
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    toolbar_ = new QToolBar(this);
    toolbar_->setMovable(false);
    layout->addWidget(toolbar_);
}

QAction* RibbonWidget::addRibbonAction(const QString& text)
{
    if (!toolbar_)
        return nullptr;

    return toolbar_->addAction(text);
}
