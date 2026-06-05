#include "occ/OccContext.h"

OccContext::OccContext(const Handle(V3d_Viewer)& viewer)
{
    if (!viewer.IsNull())
    {
        context_ = new AIS_InteractiveContext(viewer);
        context_->SetDisplayMode(AIS_Shaded, Standard_True);
        context_->SetAutomaticHilight(Standard_True);
    }
}

Handle(AIS_InteractiveContext) OccContext::context() const noexcept
{
    return context_;
}
