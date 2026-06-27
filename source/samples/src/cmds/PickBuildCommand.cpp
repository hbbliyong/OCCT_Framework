#include "cmds/PickBuildCommand.h"

#include <BRepAlgoAPI_Fuse.hxx>

#include "command/CommandContext.h"
#include "selection/SelectionManager.h"
#include "document/DocumentManager.h"
#include "common/StatusService.h"

namespace Samples
{
	bool PickBuildCommand::execute()
	{
		auto& selMgr = this->context().selectionManager();
		auto& docMgr = this->context().documentManager();
		std::vector<TopoDS_Shape> picked;

		while (true)
		{
			auto r = selMgr.PickObject("Pick an object (right-click to finish)");
			if (!r.has_value() || r->shape.IsNull()) break;
			picked.push_back(r->shape);
			this->context().statusService().showMessage(QString("Picked %1 object(s).").arg(picked.size()));
		}

		if (picked.size() < 2)
		{
			this->context().statusService().showMessage("Need at least 2 objects. Cancelled.");
			return false;
		}

		TopoDS_Shape fused = picked[0];
		for (size_t i = 1; i < picked.size(); ++i)
		{
			BRepAlgoAPI_Fuse fuseOp(fused, picked[i]);
			if (!fuseOp.IsDone())
			{
				this->context().statusService().showMessage(QString("Fuse failed at object %1.").arg(i + 1));
				return false;
			}
			fused = fuseOp.Shape();
		}

		docMgr.activeDocument()->addObject(fused);
		this->context().statusService().showMessage("Build (boolean fuse) completed.");
		return true;
	}

	REGISTER_COMMAND(PickBuildCommand, "Samples.PickBuild");

} // namespace Samples
