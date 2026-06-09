#include "tool/tools/PickPointTool.h"

#include <QMouseEvent>
#include <QKeyEvent>

#include "view/OccView.h"
namespace SongYun
{
	PickPointTool::PickPointTool(
		OccView* view)
		: m_view(view)
	{}

	void PickPointTool::MousePress(
		QMouseEvent* e)
	{
		if (e->button() == Qt::RightButton)
		{
			if (OnCancelled)
				OnCancelled();

			return;
		}

		if (e->button() != Qt::LeftButton)
			return;

		Standard_Real x, y, z;

		m_view->view()->Convert(
			e->position().x(),
			e->position().y(),
			x, y, z);

		if (OnPicked)
		{
			OnPicked(
				gp_Pnt(x, y, z));
		}
	}

	void PickPointTool::KeyPress(
		QKeyEvent* e)
	{
		if (e->key() == Qt::Key_Escape)
		{
			if (OnCancelled)
				OnCancelled();
		}
	}
}