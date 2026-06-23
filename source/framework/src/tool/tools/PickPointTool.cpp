#include "tool/tools/PickPointTool.h"

#include <QMouseEvent>
#include <QKeyEvent>

#include "view/View.h"
namespace SongYun
{
	PickPointTool::PickPointTool(
		View* view)
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

		m_view->getV3dView()->Convert(
			e->position().x() * m_view->getDPR(),
			e->position().y() * m_view->getDPR(),
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