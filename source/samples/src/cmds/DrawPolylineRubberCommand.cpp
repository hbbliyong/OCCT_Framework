#include "cmds/DrawPolylineRubberCommand.h"

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
	void DrawPolylineRubberCommand::buildUI(SongYun::CommandUI& ui)
	{
		ui.group("Polyline")
			.checkBox("closed")
				.label("Closed")
				.bind(&m_opts, &Options::closed);
	}

	bool DrawPolylineRubberCommand::execute()
	{
		auto& docMgr = this->context().documentManager();
		auto* view  = this->context().viewManager().activeView();
		std::vector<gp_Pnt> points;

		this->context().statusService().showMessage("Left-click to add vertex (rubber-band preview), right-click/Esc to finish.");

		std::optional<gp_Pnt> prev;
		while (true)
		{
			auto pt = this->context().selectionManager().PickPointWithRubber(
				"Click vertex (right-click to finish)", prev);
			if (!pt.has_value()) break;

			points.push_back(*pt);
			prev = *pt;

			if (points.size() >= 2 && view)
			{
				BRepBuilderAPI_MakeWire wm;
				for (size_t i = 1; i < points.size(); ++i)
					wm.Add(BRepBuilderAPI_MakeEdge(points[i - 1], points[i]));
				if (m_opts.closed && points.size() >= 3)
					wm.Add(BRepBuilderAPI_MakeEdge(points.back(), points.front()));
				view->updateTemporaryShape(wm.Shape());
			}

			this->context().statusService().showMessage(
				QString("Added vertex %1.").arg(points.size()));
		}

		if (view) view->clearTemporaryShape();

		if (points.size() < 2)
		{
			this->context().statusService().showMessage("Need at least 2 vertices. Cancelled.");
			return false;
		}

		BRepBuilderAPI_MakeWire wm;
		for (size_t i = 1; i < points.size(); ++i)
			wm.Add(BRepBuilderAPI_MakeEdge(points[i - 1], points[i]));
		if (m_opts.closed && points.size() >= 3)
			wm.Add(BRepBuilderAPI_MakeEdge(points.back(), points.front()));

		docMgr.activeDocument()->addObject(wm.Shape());
		this->context().statusService().showMessage("Polyline created with rubber band.");
		return true;
	}

	REGISTER_COMMAND(DrawPolylineRubberCommand, "Samples.DrawPolylineRubber");

} // namespace Samples
