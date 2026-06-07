#pragma once

#include "core/CoreMacro.hpp"
#include <TopoDS_Shape.hxx>

namespace SongYun {

struct FilletBuilder
{
    SONGYUN_API static TopoDS_Shape build(const TopoDS_Shape& shape, double radius);
};

} // namespace SongYun
