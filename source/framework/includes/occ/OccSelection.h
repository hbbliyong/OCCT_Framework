#pragma once

#include "core/CoreMacro.hpp"
#include "selection/PickResult.h"
#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>

namespace SongYun {

class OccSelection
{
public:
    SONGYUN_API explicit OccSelection(const Handle(AIS_InteractiveContext)& context);

    SONGYUN_API PickResult currentSelection() const;
    SONGYUN_API void selectAt(int x, int y, const Handle(V3d_View)& view);

private:
    Handle(AIS_InteractiveContext) context_;
};

} // namespace SongYun
