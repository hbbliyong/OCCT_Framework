#pragma once

#include "tool/Tool.h"
#include "selection/SnapEngine.h"

#include <functional>
#include <optional>
#include <gp_Pnt.hxx>
#include <AIS_Point.hxx>

namespace SongYun
{
	class View;

	class PickPointTool : public Tool
	{
	public:
		explicit PickPointTool(View* view);

		std::function<void(const gp_Pnt&)> OnPicked;
		std::function<void()> OnCancelled;

		void MouseMove(QMouseEvent* e) override;
		void MousePress(QMouseEvent* e) override;
		void KeyPress(QKeyEvent* e) override;
		void OnLeave() override;

	private:
		View*      m_view{};
		SnapEngine m_snap;
		gp_Pnt     m_snappedPoint;
		bool       m_hasSnap = false;
		Handle(AIS_Point) m_snapMarker;  // 持久化的捕捉十字
	};

}