#pragma once

#include "core/CoreMacro.hpp"
#include <TopoDS_Shape.hxx>

namespace SongYun {

struct SweepBuilder
{
    SONGYUN_API static TopoDS_Shape build(const TopoDS_Shape& profile, const TopoDS_Shape& path);
};

} // namespace SongYun
