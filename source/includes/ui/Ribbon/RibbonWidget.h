#pragma once

#include <QWidget>
#include <QString>

class QAction;
class QToolBar;

class RibbonWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RibbonWidget(QWidget *parent = nullptr);
    ~RibbonWidget() override = default;

    QAction *addRibbonAction(const QString &text);

private:
    QToolBar *toolbar_ = nullptr;
};
