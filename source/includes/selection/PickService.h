#pragma once

#include "selection/PickResult.h"
#include "view/View.h"
#include <AIS_InteractiveContext.hxx>
#include <gp_Pnt.hxx>

class PickService
{
public:
    static PickService &instance();

    void initialize(View *view, const Handle(AIS_InteractiveContext) & context);

    bool isInitialized() const noexcept;

    gp_Pnt pickPoint(int x, int y) const;
    PickResult pickObject(int x, int y) const;

private:
    PickService() = default;
    View *myView = nullptr;
    Handle(AIS_InteractiveContext) myContext;
};
