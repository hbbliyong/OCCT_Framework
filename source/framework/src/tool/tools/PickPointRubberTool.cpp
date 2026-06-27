#include "tool/tools/PickPointRubberTool.h"

#include <QMouseEvent>
#include <QKeyEvent>

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>
#include <Prs3d_LineAspect.hxx>
#include <AIS_Line.hxx>

#include "view/View.h"

namespace SongYun
{
	PickPointRubberTool::PickPointRubberTool(View* view)
		: m_view(view)
	{
	}

	void PickPointRubberTool::SetAnchorPoint(const gp_Pnt& anchor)
	{
		m_anchorPoint = anchor;
	}

	void PickPointRubberTool::ClearAnchor()
	{
		clearRubberLine();
		m_anchorPoint.reset();
	}

	bool PickPointRubberTool::HasAnchor() const
	{
		return m_anchorPoint.has_value();
	}

	void PickPointRubberTool::OnEnter()
	{
		// 工具激活时无需特殊处理
	}

	void PickPointRubberTool::OnLeave()
	{
		// 工具停用时清除橡皮线
		clearRubberLine();
	}

	void PickPointRubberTool::MouseMove(QMouseEvent* e)
	{
		if (!m_anchorPoint.has_value())
			return;

		const gp_Pnt currentPoint = convertScreenToWorld(e);
		updateRubberLine(currentPoint);
	}

	void PickPointRubberTool::MousePress(QMouseEvent* e)
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

		const gp_Pnt worldPoint = convertScreenToWorld(e);

		// 更新锚点为当前拾取点（使橡皮线从新点继续）
		m_anchorPoint = worldPoint;

		if (OnPicked)
			OnPicked(worldPoint);
	}

	void PickPointRubberTool::KeyPress(QKeyEvent* e)
	{
		if (e->key() == Qt::Key_Escape)
		{
			if (OnCancelled)
				OnCancelled();
		}
	}

	gp_Pnt PickPointRubberTool::convertScreenToWorld(QMouseEvent* e) const
	{
		Standard_Real x, y, z;
		m_view->getV3dView()->Convert(
			e->position().x() * m_view->getDPR(),
			e->position().y() * m_view->getDPR(),
			x, y, z);
		return gp_Pnt(x, y, z);
	}

	void PickPointRubberTool::updateRubberLine(const gp_Pnt& currentPoint)
	{
		if (!m_anchorPoint.has_value())
			return;

		auto& ctx = m_view->getContext();

		// 先移除旧的橡皮线
		if (!m_rubberLine.IsNull())
		{
			ctx->Remove(m_rubberLine, Standard_False);
			m_rubberLine.Nullify();
		}

		// 创建从锚点到当前鼠标位置的新线段
		TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(
			m_anchorPoint.value(), currentPoint);

		if (!edge.IsNull())
		{
			m_rubberLine = new AIS_Shape(edge);

			// 设置橡皮线样式：白色虚线
			Handle(Prs3d_Drawer) drawer = m_rubberLine->Attributes();
			Handle(Prs3d_LineAspect) lineAspect = new Prs3d_LineAspect(
				Quantity_NOC_YELLOW, Aspect_TOL_DASH, 1.0);
			drawer->SetLineAspect(lineAspect);
			drawer->SetWireDraw(Standard_True);

			ctx->Display(m_rubberLine, Standard_False);
			ctx->Deactivate(m_rubberLine); // 不可被选中
		}
	}

	void PickPointRubberTool::clearRubberLine()
	{
		if (!m_rubberLine.IsNull())
		{
			auto& ctx = m_view->getContext();
			ctx->Remove(m_rubberLine, Standard_False);
			m_rubberLine.Nullify();
		}
	}

} // namespace SongYun
