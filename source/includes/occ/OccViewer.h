#pragma once

#include <V3d_Viewer.hxx>
#include <OpenGl_GraphicDriver.hxx>

class OccViewer
{
public:
    explicit OccViewer(const Handle(OpenGl_GraphicDriver)& driver);

    Handle(V3d_Viewer) viewer() const noexcept;

private:
    Handle(V3d_Viewer) viewer_;
};
