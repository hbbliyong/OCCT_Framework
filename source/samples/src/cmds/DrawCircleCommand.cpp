#include "cmds/DrawCircleCommand.h"

#include <GC_MakeCircle.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <gp_Circ.hxx>

#include "command/CommandContext.h"
#include "selection/SelectionManager.h"
#include "view/ViewManager.h"
#include "view/View.h"
#include "document/DocumentManager.h"
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
		auto& docMgr = this->context().documentManager();
		auto* view  = this->context().viewManager().activeView();

		auto center = this->context().selectionManager().PickPoint("Click circle center.");
		if (!center.has_value()) { this->context().statusService().showMessage("Cancelled."); return false; }

		// 在 PickPointWithRubber 期间手动更新预览圆
		// 由于 PickPointWithRubber 不支持自定义预览形状，这里用一个折中方案：
		// 使用 PickPointWithRubber 显示半径线，最终点确定半径
		auto onEdge = this->context().selectionManager().PickPointWithRubber(
			"Click point on circumference.", *center);
		if (!onEdge.has_value()) { this->context().statusService().showMessage("Cancelled."); return false; }

		double radius = center->Distance(*onEdge);
		gp_Circ circ(gp_Ax2(*center, gp_Dir(0, 0, 1)), radius);
		auto edge = BRepBuilderAPI_MakeEdge(circ).Shape();

		docMgr.activeDocument()->addObject(edge);
		this->context().statusService().showMessage("Circle created.");
		return true;
	}

	bool DrawCircleCommand::executeTwoPoints()
	{
		auto& docMgr = this->context().documentManager();

		auto p1 = this->context().selectionManager().PickPoint("Click first point (diameter).");
		if (!p1.has_value()) { this->context().statusService().showMessage("Cancelled."); return false; }

		auto p2 = this->context().selectionManager().PickPointWithRubber(
			"Click second point (diameter).", *p1);
		if (!p2.has_value()) { this->context().statusService().showMessage("Cancelled."); return false; }

		gp_Pnt mid((p1->X() + p2->X()) / 2, (p1->Y() + p2->Y()) / 2, (p1->Z() + p2->Z()) / 2);
		double radius = p1->Distance(*p2) / 2.0;
		gp_Circ circ(gp_Ax2(mid, gp_Dir(0, 0, 1)), radius);
		auto edge = BRepBuilderAPI_MakeEdge(circ).Shape();

		docMgr.activeDocument()->addObject(edge);
		this->context().statusService().showMessage("Circle created.");
		return true;
	}

	bool DrawCircleCommand::executeThreePoints()
	{
		auto& docMgr = this->context().documentManager();

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

		docMgr.activeDocument()->addObject(BRepBuilderAPI_MakeEdge(circ).Shape());
		this->context().statusService().showMessage("Circle created.");
		return true;
	}

	REGISTER_COMMAND(DrawCircleCommand, "Samples.DrawCircle");

} // namespace Samples
