#include "tool/tools/PickObjectTool.h"

#include <QMouseEvent>
#include <QKeyEvent>

#include "view/View.h"
#include "selection/SelectionFilter.h"

namespace SongYun
{
	PickObjectTool::PickObjectTool(View* view)
		: m_view(view)
	{
	}

	void PickObjectTool::SetFilter(std::shared_ptr<SelectionFilter> filter)
	{
		m_filter = std::move(filter);
	}

	void PickObjectTool::MousePress(QMouseEvent* e)
	{
		// 右键取消
		if (e->button() == Qt::RightButton)
		{
			if (OnCancelled)
				OnCancelled();
			return;
		}

		// 仅左键拾取
		if (e->button() != Qt::LeftButton)
			return;

		auto& ctx = m_view->getContext();

		const int x = static_cast<int>(e->position().x() * m_view->getDPR());
		const int y = static_cast<int>(e->position().y() * m_view->getDPR());

		// 使用 OCCT 的检测机制：将鼠标移动到指定位置并检测对象
		ctx->MoveTo(x, y, m_view->getV3dView(), Standard_False);

		if (ctx->HasDetected())
		{
			PickResult result;
			result.object = ctx->DetectedInteractive();
			result.shape = ctx->DetectedShape();

			// 检查过滤器
			if (m_filter && !result.shape.IsNull())
			{
				if (!m_filter->accept(result.shape))
				{
					// 不符合过滤条件，忽略此次拾取
					return;
				}
			}

			// 转换屏幕坐标到世界点
			Standard_Real wx, wy, wz;
			m_view->getV3dView()->Convert(x, y, wx, wy, wz);
			result.worldPoint = gp_Pnt(wx, wy, wz);

			if (OnPicked)
				OnPicked(result);
		}
	}

	void PickObjectTool::KeyPress(QKeyEvent* e)
	{
		if (e->key() == Qt::Key_Escape)
		{
			if (OnCancelled)
				OnCancelled();
		}
	}

} // namespace SongYun
