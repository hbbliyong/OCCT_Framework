#pragma once

#include "core/CoreMacro.hpp"
#include <QObject>

#include "V3d_View.hxx"

namespace SongYun {

class CameraController : public QObject
{
    Q_OBJECT

public:
    SONGYUN_API explicit CameraController(const Handle(V3d_View)& view, QObject* parent = nullptr);

    SONGYUN_API void fitAll();
    SONGYUN_API void setProjection(Graphic3d_Camera::Projection projection);

private:
    Handle(V3d_View) view_;
};

} // namespace SongYun
