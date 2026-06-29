#include "cmds/DrawCircleCommand.h"

#include "entity/syCircle.h"

#include <GC_MakeCircle.hxx>
#include <gp_Circ.hxx>

#include "command/CommandContext.h"
#include "selection/SelectionManager.h"
#include "document/DocumentManager.h"
#include "document/Document.h"
#include "common/StatusService.h"

namespace Samples
{
	void DrawCircleCommand::buildUI(SongYun::CommandUI& ui)
	{
		ui.group("Circle")
			.combo("mode")
				.label("Mode")
				.items({ "Center+Radius", "2 Points", "3 Points" })
				.bind(&m_opts, &Options::mode);
	}

	bool DrawCircleCommand::execute()
	{
		switch (m_opts.mode)
		{
		case 0: return executeCenterRadius();
		case 1: return executeTwoPoints();
		case 2: return executeThreePoints();
		}
		return false;
	}

	bool DrawCircleCommand::executeCenterRadius()
	{
		auto* doc = this->context().documentManager().activeDocument().get();

		auto center = this->context().selectionManager().PickPoint("Click circle center.");
		if (!center.has_value()) { this->context().statusService().showMessage("Cancelled."); return false; }

		auto onEdge = this->context().selectionManager().PickPointWithRubber(
			"Click point on circumference.", *center);
		if (!onEdge.has_value()) { this->context().statusService().showMessage("Cancelled."); return false; }

		auto circle = std::make_shared<SongYun::SyCircle>();
		circle->center.set(*center);
		circle->radius.set(center->Distance(*onEdge));
		doc->addCadObject(circle);
		doc->commit();

		this->context().statusService().showMessage("Circle created.");
		return true;
	}

	bool DrawCircleCommand::executeTwoPoints()
	{
		auto* doc = this->context().documentManager().activeDocument().get();

		auto p1 = this->context().selectionManager().PickPoint("Click first point (diameter).");
		if (!p1.has_value()) { this->context().statusService().showMessage("Cancelled."); return false; }

		auto p2 = this->context().selectionManager().PickPointWithRubber(
			"Click second point (diameter).", *p1);
		if (!p2.has_value()) { this->context().statusService().showMessage("Cancelled."); return false; }

		gp_Pnt mid((p1->X() + p2->X()) / 2, (p1->Y() + p2->Y()) / 2, (p1->Z() + p2->Z()) / 2);

		auto circle = std::make_shared<SongYun::SyCircle>();
		circle->center.set(mid);
		circle->radius.set(p1->Distance(*p2) / 2.0);
		doc->addCadObject(circle);
		doc->commit();

		this->context().statusService().showMessage("Circle created.");
		return true;
	}

	bool DrawCircleCommand::executeThreePoints()
	{
		auto* doc = this->context().documentManager().activeDocument().get();

		auto p1 = this->context().selectionManager().PickPoint("Click first point on circle.");
		if (!p1.has_value()) { this->context().statusService().showMessage("Cancelled."); return false; }

		auto p2 = this->context().selectionManager().PickPointWithRubber(
			"Click second point on circle.", *p1);
		if (!p2.has_value()) { this->context().statusService().showMessage("Cancelled."); return false; }

		auto p3 = this->context().selectionManager().PickPointWithRubber(
			"Click third point on circle.", *p2);
		if (!p3.has_value()) { this->context().statusService().showMessage("Cancelled."); return false; }

		Handle(Geom_Circle) circ = GC_MakeCircle(*p1, *p2, *p3);
		if (circ.IsNull()) { this->context().statusService().showMessage("Invalid circle."); return false; }

		auto circle = std::make_shared<SongYun::SyCircle>();
		circle->center.set(circ->Location());
		circle->radius.set(circ->Radius());
		doc->addCadObject(circle);
		doc->commit();

		this->context().statusService().showMessage("Circle created.");
		return true;
	}

	REGISTER_COMMAND(DrawCircleCommand, "Samples.DrawCircle");

} // namespace Samples
