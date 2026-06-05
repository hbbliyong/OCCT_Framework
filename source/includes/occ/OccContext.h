#pragma once

#include <AIS_InteractiveContext.hxx>
#include <V3d_Viewer.hxx>
#include <OpenGl_GraphicDriver.hxx>

class OccContext
{
public:
    explicit OccContext(const Handle(V3d_Viewer)& viewer);

    Handle(AIS_InteractiveContext) context() const noexcept;

private:
    Handle(AIS_InteractiveContext) context_;
};
