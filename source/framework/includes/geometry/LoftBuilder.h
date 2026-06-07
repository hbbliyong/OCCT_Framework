#pragma once

#include "core/CoreMacro.hpp"
#include <TopoDS_Shape.hxx>
#include <vector>

namespace SongYun {

struct LoftBuilder
{
    SONGYUN_API static TopoDS_Shape build(const std::vector<TopoDS_Shape>& profiles);
};

} // namespace SongYun
