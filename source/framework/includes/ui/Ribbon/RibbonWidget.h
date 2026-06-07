#pragma once

#include "core/CoreMacro.hpp"
#include <QWidget>
#include <QString>

class QAction;
class QToolBar;

namespace SongYun {

class RibbonWidget : public QWidget
{
    Q_OBJECT

public:
    SONGYUN_API explicit RibbonWidget(QWidget *parent = nullptr);
    SONGYUN_API ~RibbonWidget() override = default;

    SONGYUN_API QAction *addRibbonAction(const QString &text);

private:
    QToolBar *toolbar_ = nullptr;
};

} // namespace SongYun
