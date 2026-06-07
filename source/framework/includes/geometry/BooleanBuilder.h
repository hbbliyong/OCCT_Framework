#pragma once

#include "core/CoreMacro.hpp"
#include <TopoDS_Shape.hxx>

namespace SongYun {

struct BooleanBuilder
{
    SONGYUN_API static TopoDS_Shape cut(const TopoDS_Shape& left, const TopoDS_Shape& right);
    SONGYUN_API static TopoDS_Shape fuse(const TopoDS_Shape& left, const TopoDS_Shape& right);
    SONGYUN_API static TopoDS_Shape common(const TopoDS_Shape& left, const TopoDS_Shape& right);
};

} // namespace SongYun
