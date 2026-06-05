#pragma once

#include <TopoDS_Shape.hxx>
#include <vector>

struct LoftBuilder
{
    static TopoDS_Shape build(const std::vector<TopoDS_Shape>& profiles);
};
