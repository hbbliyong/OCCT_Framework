#pragma once

#include <optional>

#include <TopoDS_Shape.hxx>
#include <AIS_InteractiveObject.hxx>
#include <gp_Pnt.hxx>

struct PickResult
{
    gp_Pnt worldPoint;

    TopoDS_Shape shape;

    Handle(AIS_InteractiveObject) object;
};
