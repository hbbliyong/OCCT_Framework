#pragma once

#include "core/CoreMacro.hpp"
#include <AIS_InteractiveContext.hxx>
#include <V3d_Viewer.hxx>
#include <OpenGl_GraphicDriver.hxx>

namespace SongYun {

class OccContext
{
public:
    SONGYUN_API explicit OccContext(const Handle(V3d_Viewer)& viewer);

    SONGYUN_API Handle(AIS_InteractiveContext) context() const noexcept;

private:
    Handle(AIS_InteractiveContext) context_;
};

} // namespace SongYun
