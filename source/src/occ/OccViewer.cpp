#include "occ/OccViewer.h"

OccViewer::OccViewer(const Handle(OpenGl_GraphicDriver)& driver)
{
    if (!driver.IsNull())
    {
        viewer_ = new V3d_Viewer(driver);
        viewer_->SetDefaultLights();
        viewer_->SetLightOn();
    }
}

Handle(V3d_Viewer) OccViewer::viewer() const noexcept
{
    return viewer_;
}
