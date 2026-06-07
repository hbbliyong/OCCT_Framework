#pragma once

#include "core/CoreMacro.hpp"
#include <optional>

#include <TopoDS_Shape.hxx>
#include <AIS_InteractiveObject.hxx>
#include <gp_Pnt.hxx>

namespace SongYun {

struct PickResult
{
    gp_Pnt worldPoint;

    TopoDS_Shape shape;

    Handle(AIS_InteractiveObject) object;
};

} // namespace SongYun
