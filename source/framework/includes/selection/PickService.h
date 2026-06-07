#pragma once

#include "core/CoreMacro.hpp"
#include "selection/PickResult.h"
#include "view/View.h"
#include <AIS_InteractiveContext.hxx>
#include <gp_Pnt.hxx>

namespace SongYun {

class PickService
{
public:
    SONGYUN_API static PickService &instance();

    SONGYUN_API void initialize(View *view, const Handle(AIS_InteractiveContext) & context);

    SONGYUN_API bool isInitialized() const noexcept;

    SONGYUN_API gp_Pnt pickPoint(int x, int y) const;
    SONGYUN_API PickResult pickObject(int x, int y) const;

private:
    PickService() = default;
    View *myView = nullptr;
    Handle(AIS_InteractiveContext) myContext;
};

} // namespace SongYun
