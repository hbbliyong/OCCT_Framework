#pragma once

#include "core/CoreMacro.hpp"
#include <V3d_Viewer.hxx>
#include <OpenGl_GraphicDriver.hxx>

namespace SongYun {

class OccViewer
{
public:
    SONGYUN_API explicit OccViewer(const Handle(OpenGl_GraphicDriver)& driver);

    SONGYUN_API Handle(V3d_Viewer) viewer() const noexcept;

private:
    Handle(V3d_Viewer) viewer_;
};

} // namespace SongYun
