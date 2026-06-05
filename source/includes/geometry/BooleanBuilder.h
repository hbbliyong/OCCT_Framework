#pragma once

#include <TopoDS_Shape.hxx>

struct BooleanBuilder
{
    static TopoDS_Shape cut(const TopoDS_Shape& left, const TopoDS_Shape& right);
    static TopoDS_Shape fuse(const TopoDS_Shape& left, const TopoDS_Shape& right);
    static TopoDS_Shape common(const TopoDS_Shape& left, const TopoDS_Shape& right);
};
