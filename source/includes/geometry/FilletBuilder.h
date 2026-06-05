#pragma once

#include <TopoDS_Shape.hxx>

struct FilletBuilder
{
    static TopoDS_Shape build(const TopoDS_Shape& shape, double radius);
};
