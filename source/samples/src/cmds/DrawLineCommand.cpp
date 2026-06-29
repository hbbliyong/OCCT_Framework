#include "cmds/DrawLineCommand.h"

#include "entity/syLine.h"

#include "command/CommandContext.h"
#include "selection/SelectionManager.h"
#include "document/DocumentManager.h"
#include "document/Document.h"
#include "common/StatusService.h"

namespace Samples
{
	bool DrawLineCommand::execute()
	{
		auto* doc = this->context().documentManager().activeDocument().get();

		auto p1 = this->context().selectionManager().PickPoint("Click start point.");
		if (!p1.has_value()) { this->context().statusService().showMessage("Cancelled."); return false; }

		auto p2 = this->context().selectionManager().PickPointWithRubber("Click end point.", *p1);
		if (!p2.has_value()) { this->context().statusService().showMessage("Cancelled."); return false; }

		auto line = std::make_shared<SongYun::SyLine>();
		line->start.set(*p1);
		line->end.set(*p2);
		doc->addCadObject(line);
		doc->commit();

		this->context().statusService().showMessage("Line created.");
		return true;
	}

	REGISTER_COMMAND(DrawLineCommand, "Samples.DrawLine");

} // namespace Samples
