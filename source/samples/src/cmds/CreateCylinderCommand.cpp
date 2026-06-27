#include "cmds/CreateCylinderCommand.h"

#include <BRepPrimAPI_MakeCylinder.hxx>
#include <gp_Ax2.hxx>
#include <gp_Dir.hxx>

#include "command/CommandContext.h"
#include "selection/SelectionManager.h"
#include "document/DocumentManager.h"
#include "common/StatusService.h"

namespace Samples
{
	bool CreateCylinderCommand::execute()
	{
		auto pt = this->context().selectionManager().PickPoint("Click to place the cylinder center.");
		if (!pt.has_value())
		{
			this->context().statusService().showMessage("Cancelled.");
			return false;
		}

		auto shape = BRepPrimAPI_MakeCylinder(gp_Ax2(*pt, gp_Dir(0, 0, 1)), 20.0, 120.0).Shape();
		this->context().documentManager().activeDocument()->addObject(shape);

		this->context().statusService().showMessage("Cylinder created.");
		return true;
	}

	REGISTER_COMMAND(CreateCylinderCommand, "Samples.CreateCylinder");

} // namespace Samples
