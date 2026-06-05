#pragma once

#include <TopoDS_Shape.hxx>

class SelectionFilter
{
public:
    virtual ~SelectionFilter() = default;
    virtual bool accept(const TopoDS_Shape& shape) const = 0;
};
