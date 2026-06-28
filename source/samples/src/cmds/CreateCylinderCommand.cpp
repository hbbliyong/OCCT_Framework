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
	void CreateCylinderCommand::buildUI(SongYun::CommandUI& ui)
	{
		ui.group("Cylinder")
			.doubleSpin("radius")
				.label("Radius")
				.range(1.0, 200.0)
				.step(1.0)
				.bind(&m_opts, &Options::radius)
			.doubleSpin("height")
				.label("Height")
				.range(1.0, 500.0)
				.step(1.0)
				.bind(&m_opts, &Options::height);
	}

	bool CreateCylinderCommand::execute()
	{
		auto pt = this->context().selectionManager().PickPoint("Click to place the cylinder center.");
		if (!pt.has_value())
		{
			this->context().statusService().showMessage("Cancelled.");
			return false;
		}

		auto shape = BRepPrimAPI_MakeCylinder(
			gp_Ax2(*pt, gp_Dir(0, 0, 1)), m_opts.radius, m_opts.height).Shape();
		this->context().documentManager().activeDocument()->addObject(shape);

		this->context().statusService().showMessage("Cylinder created.");
		return true;
	}

	REGISTER_COMMAND(CreateCylinderCommand, "Samples.CreateCylinder");

} // namespace Samples
