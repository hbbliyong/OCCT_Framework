#pragma once

#include "selection/PickResult.h"
#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>

class OccSelection
{
public:
    explicit OccSelection(const Handle(AIS_InteractiveContext)& context);

    PickResult currentSelection() const;
    void selectAt(int x, int y, const Handle(V3d_View)& view);

private:
    Handle(AIS_InteractiveContext) context_;
};
