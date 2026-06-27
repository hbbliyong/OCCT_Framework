#pragma once

#include "tool/Tool.h"

#include <functional>
#include <optional>
#include <gp_Pnt.hxx>
#include <AIS_Shape.hxx>

namespace SongYun
{
	class View;

	/// @brief 带橡皮线的拾取点工具
	/// 在鼠标移动时显示从锚点到当前鼠标位置的预览线（橡皮线），左键确认点，右键/Esc取消
	class PickPointRubberTool : public Tool
	{
	public:
		explicit PickPointRubberTool(View* view);

		/// 拾取点回调
		std::function<void(const gp_Pnt&)> OnPicked;

		/// 取消回调
		std::function<void()> OnCancelled;

		/// 设置橡皮线的锚点（起点）
		void SetAnchorPoint(const gp_Pnt& anchor);

		/// 清除锚点（隐藏橡皮线）
		void ClearAnchor();

		/// 是否有锚点
		bool HasAnchor() const;

		void OnEnter() override;
		void OnLeave() override;
		void MouseMove(QMouseEvent* e) override;
		void MousePress(QMouseEvent* e) override;
		void KeyPress(QKeyEvent* e) override;

	private:
		gp_Pnt convertScreenToWorld(QMouseEvent* e) const;
		void updateRubberLine(const gp_Pnt& currentPoint);
		void clearRubberLine();

	private:
		View* m_view{};
		std::optional<gp_Pnt> m_anchorPoint;
		Handle(AIS_Shape) m_rubberLine;
	};

} // namespace SongYun
