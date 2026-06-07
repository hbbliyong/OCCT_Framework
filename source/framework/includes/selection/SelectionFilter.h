#pragma once

#include "core/CoreMacro.hpp"
#include <TopoDS_Shape.hxx>

namespace SongYun {

class SelectionFilter
{
public:
    virtual SONGYUN_API ~SelectionFilter() = default;
    virtual SONGYUN_API bool accept(const TopoDS_Shape& shape) const = 0;
};

} // namespace SongYun
