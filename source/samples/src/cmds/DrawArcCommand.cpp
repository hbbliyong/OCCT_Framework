#include "cmds/DrawArcCommand.h"

#include <GC_MakeArcOfCircle.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>

#include "command/CommandContext.h"
#include "selection/SelectionManager.h"
#include "view/ViewManager.h"
#include "view/View.h"
#include "document/DocumentManager.h"
#include "common/StatusService.h"

namespace Samples
{
	bool DrawArcCommand::execute()
	{
		auto& docMgr = this->context().documentManager();
		auto* view  = this->context().viewManager().activeView();

		auto p1 = this->context().selectionManager().PickPoint("Click first point on arc.");
		if (!p1.has_value()) { this->context().statusService().showMessage("Cancelled."); return false; }

		// 预览线：从 p1 到鼠标
		auto p2 = this->context().selectionManager().PickPointWithRubber("Click second point on arc.", *p1);
		if (!p2.has_value()) { this->context().statusService().showMessage("Cancelled."); return false; }

		// 预览弧线：从 p1 到 p2（中间点）
		auto p3 = this->context().selectionManager().PickPointWithRubber("Click third point on arc.", *p2);
		if (!p3.has_value()) { this->context().statusService().showMessage("Cancelled."); return false; }

		Handle(Geom_TrimmedCurve) arc = GC_MakeArcOfCircle(*p1, *p2, *p3);
		if (arc.IsNull()) { this->context().statusService().showMessage("Invalid arc."); return false; }

		docMgr.activeDocument()->addObject(BRepBuilderAPI_MakeEdge(arc).Shape());
		this->context().statusService().showMessage("Arc created.");
		return true;
	}

	REGISTER_COMMAND(DrawArcCommand, "Samples.DrawArc");

} // namespace Samples
