#include "cmds/SetViewCommand.h"

#include "command/CommandContext.h"
#include "view/ViewManager.h"
#include "view/View.h"
#include "common/StatusService.h"

namespace Samples
{
	QString SetViewCommand::name() const
	{
		switch (m_mode)
		{
		case Axo:   return "View3D";
		case Top:   return "ViewXOY";
		case Front: return "ViewXOZ";
		case Left:  return "ViewYOZ";
		}
		return "SetView";
	}

	bool SetViewCommand::execute()
	{
		auto* view = this->context().viewManager().activeView();
		if (!view) return false;

		switch (m_mode)
		{
		case Axo:   view->setAxo();   break;
		case Top:   view->setTop();   break;
		case Front: view->setFront(); break;
		case Left:  view->setLeft();  break;
		}

		this->context().statusService().showMessage(
			QString("View: %1").arg(name()));
		return true;
	}

} // namespace Samples
