#include "cmds/DrawLineCommand.h"

#include <BRepBuilderAPI_MakeEdge.hxx>

#include "command/CommandContext.h"
#include "selection/SelectionManager.h"
#include "document/DocumentManager.h"
#include "common/StatusService.h"

namespace Samples
{
	bool DrawLineCommand::execute()
	{
		auto& docMgr = this->context().documentManager();

		this->context().statusService().showMessage("Click start point.");
		auto p1 = this->context().selectionManager().PickPoint("Click start point.");
		if (!p1.has_value()) { this->context().statusService().showMessage("Cancelled."); return false; }

		this->context().statusService().showMessage("Click end point.");
		auto p2 = this->context().selectionManager().PickPointWithRubber("Click end point.", *p1);
		if (!p2.has_value()) { this->context().statusService().showMessage("Cancelled."); return false; }

		auto edge = BRepBuilderAPI_MakeEdge(*p1, *p2).Shape();
		docMgr.activeDocument()->addObject(edge);
		this->context().statusService().showMessage("Line created.");
		return true;
	}

	REGISTER_COMMAND(DrawLineCommand, "Samples.DrawLine");

} // namespace Samples
