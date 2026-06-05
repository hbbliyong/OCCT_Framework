#include "view/CameraController.h"

CameraController::CameraController(const Handle(V3d_View)& view, QObject* parent)
    : QObject(parent), view_(view)
{
}

void CameraController::fitAll()
{
    if (!view_.IsNull())
    {
        view_->FitAll();
        view_->ZFitAll();
        view_->Redraw();
    }
}

void CameraController::setProjection(Graphic3d_Camera::Projection projection)
{
    if (!view_.IsNull())
    {
        view_->Camera()->SetProjectionType(projection);
        view_->Redraw();
    }
}
