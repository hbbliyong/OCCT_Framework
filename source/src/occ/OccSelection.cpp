#include "occ/OccSelection.h"

OccSelection::OccSelection(const Handle(AIS_InteractiveContext)& context)
    : context_(context)
{
}

PickResult OccSelection::currentSelection() const
{
    PickResult result;
    if (context_.IsNull())
        return result;

    context_->InitSelected();
    if (context_->MoreSelected())
    {
        result.object = context_->SelectedInteractive();
        result.shape = context_->SelectedShape();
    }
    return result;
}

void OccSelection::selectAt(int x, int y, const Handle(V3d_View)& view)
{
    if (context_.IsNull() || view.IsNull())
        return;

    context_->MoveTo(x, y, view, Standard_True);
    if (context_->MoreDetected())
    {
        context_->SelectDetected(AIS_SelectionScheme_Replace);
    }
}
