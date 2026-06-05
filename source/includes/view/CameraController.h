#pragma once

#include <QObject>

#include "V3d_View.hxx"

class CameraController : public QObject
{
    Q_OBJECT

public:
    explicit CameraController(const Handle(V3d_View)& view, QObject* parent = nullptr);

    void fitAll();
    void setProjection(Graphic3d_Camera::Projection projection);

private:
    Handle(V3d_View) view_;
};
