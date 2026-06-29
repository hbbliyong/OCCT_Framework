#include "cmds/CreatePolylineCommand.h"

#include "entity/syPolyline.h"

#include "command/CommandContext.h"
#include "selection/SelectionManager.h"
#include "view/ViewManager.h"
#include "view/View.h"
#include "document/DocumentManager.h"
#include "document/Document.h"
#include "common/StatusService.h"

namespace Samples
{
	bool CreatePolylineCommand::execute()
	{
		auto* doc  = this->context().documentManager().activeDocument().get();
		auto* view = this->context().viewManager().activeView();
		std::vector<gp_Pnt> points;

		while (true)
		{
			auto pt = this->context().selectionManager().PickPoint("Click vertex (right-click to finish)");
			if (!pt.has_value()) break;
			points.push_back(*pt);

			if (points.size() >= 2 && view)
			{
				BRepBuilderAPI_MakeWire wm;
				for (size_t i = 1; i < points.size(); ++i)
					wm.Add(BRepBuilderAPI_MakeEdge(points[i - 1], points[i]));
				view->updateTemporaryShape(wm.Shape());
			}

			this->context().statusService().showMessage(QString("Added vertex %1.").arg(points.size()));
		}

		if (view) view->clearTemporaryShape();

		if (points.size() < 2)
		{
			this->context().statusService().showMessage("Need at least 2 vertices. Cancelled.");
			return false;
		}

		auto poly = std::make_shared<SongYun::SyPolyline>();
		for (auto& p : points) poly->addPoint(p);
		doc->addCadObject(poly);
		doc->commit();

		this->context().statusService().showMessage("Polyline created.");
		return true;
	}

	REGISTER_COMMAND(CreatePolylineCommand, "Samples.CreatePolyline");

} // namespace Samples
