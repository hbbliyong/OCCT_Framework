#include "tool/tools/PickPointTool.h"

#include <QMouseEvent>
#include <QKeyEvent>

#include "view/View.h"
#include <AIS_Point.hxx>
#include <Geom_CartesianPoint.hxx>
#include <Prs3d_PointAspect.hxx>
#include <Graphic3d_AspectMarker3d.hxx>
#include <AIS_InteractiveContext.hxx>

namespace SongYun
{
	PickPointTool::PickPointTool(View* view)
		: m_view(view)
	{}

	// ================================================================
	// MouseMove — 实时捕捉 + 持久化十字准星
	// ================================================================
	void PickPointTool::MouseMove(QMouseEvent* e)
	{
		if (!m_view) return;

		auto ctx = m_view->getContext();
		double dpr = m_view->getDPR();
		auto snap = m_snap.findSnap(m_view,
			{ static_cast<int>(e->position().x() * dpr),
			  static_cast<int>(e->position().y() * dpr) }, 12);

		if (snap.valid)
		{
			m_hasSnap = true;
			m_snappedPoint = snap.point;

			// 创建或更新持久化的捕捉十字准星
			if (m_snapMarker.IsNull())
			{
				m_snapMarker = new AIS_Point(new Geom_CartesianPoint(m_snappedPoint));
				m_snapMarker->SetColor(Quantity_NOC_YELLOW);
				m_snapMarker->Attributes()->PointAspect()->SetTypeOfMarker(Aspect_TOM_PLUS);
				m_snapMarker->Attributes()->PointAspect()->SetScale(3.0);
				ctx->Display(m_snapMarker, Standard_False);
			}
			else
			{
				Handle(Geom_CartesianPoint) gp = new Geom_CartesianPoint(m_snappedPoint);
				m_snapMarker->SetComponent(gp);
				ctx->Redisplay(m_snapMarker, Standard_False);
			}
			ctx->UpdateCurrentViewer();
		}
		else if (m_hasSnap)
		{
			// 移出捕捉范围，清除
			m_hasSnap = false;
			if (!m_snapMarker.IsNull())
			{
				ctx->Remove(m_snapMarker, Standard_False);
				m_snapMarker.Nullify();
				ctx->UpdateCurrentViewer();
			}
		}

		// 捕捉到特征点时显示十字光标
		m_view->setCursor(m_hasSnap ? Qt::CrossCursor : Qt::ArrowCursor);
	}

	// ================================================================
	// MousePress — 优先返回捕捉点
	// ================================================================
	void PickPointTool::MousePress(QMouseEvent* e)
	{
		if (e->button() == Qt::RightButton)
		{
			if (OnCancelled) OnCancelled();
			return;
		}

		if (e->button() != Qt::LeftButton)
			return;

		if (m_hasSnap)
		{
			if (OnPicked) OnPicked(m_snappedPoint);
			return;
		}

		Standard_Real x, y, z;
		m_view->getV3dView()->Convert(
			e->position().x() * m_view->getDPR(),
			e->position().y() * m_view->getDPR(),
			x, y, z);

		if (OnPicked)
			OnPicked(gp_Pnt(x, y, z));
	}

	void PickPointTool::KeyPress(QKeyEvent* e)
	{
		if (e->key() == Qt::Key_Escape)
		{
			if (OnCancelled) OnCancelled();
		}
	}

	void PickPointTool::OnLeave()
	{
		m_hasSnap = false;
		if (!m_snapMarker.IsNull())
		{
			m_view->getContext()->Remove(m_snapMarker, Standard_False);
			m_snapMarker.Nullify();
			m_view->getContext()->UpdateCurrentViewer();
		}
	}
}