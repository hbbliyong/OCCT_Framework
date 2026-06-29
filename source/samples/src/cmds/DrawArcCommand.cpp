#include "cmds/DrawArcCommand.h"

#include "entity/syArc.h"

#include "command/CommandContext.h"
#include "selection/SelectionManager.h"
#include "document/DocumentManager.h"
#include "document/Document.h"
#include "common/StatusService.h"

namespace Samples
{
	bool DrawArcCommand::execute()
	{
		auto* doc = this->context().documentManager().activeDocument().get();

		auto p1 = this->context().selectionManager().PickPoint("Click first point on arc.");
		if (!p1.has_value()) { this->context().statusService().showMessage("Cancelled."); return false; }

		auto p2 = this->context().selectionManager().PickPointWithRubber("Click second point on arc.", *p1);
		if (!p2.has_value()) { this->context().statusService().showMessage("Cancelled."); return false; }

		auto p3 = this->context().selectionManager().PickPointWithRubber("Click third point on arc.", *p2);
		if (!p3.has_value()) { this->context().statusService().showMessage("Cancelled."); return false; }

		auto arc = std::make_shared<SongYun::SyArc>();
		arc->start.set(*p1);
		arc->mid.set(*p2);
		arc->end.set(*p3);
		doc->addCadObject(arc);
		doc->commit();

		this->context().statusService().showMessage("Arc created.");
		return true;
	}

	REGISTER_COMMAND(DrawArcCommand, "Samples.DrawArc");

} // namespace Samples
