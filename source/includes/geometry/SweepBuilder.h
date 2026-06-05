#pragma once

#include <TopoDS_Shape.hxx>

struct SweepBuilder
{
    static TopoDS_Shape build(const TopoDS_Shape& profile, const TopoDS_Shape& path);
};
