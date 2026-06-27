#pragma once

#include "tool/Tool.h"
#include "selection/PickResult.h"

#include <functional>
#include <AIS_InteractiveContext.hxx>
#include <memory>

namespace SongYun
{
	class View;
	class SelectionFilter;

	/// @brief 拾取对象工具
	/// 左键点击拾取场景中的对象，右键/Esc取消
	class PickObjectTool : public Tool
	{
	public:
		explicit PickObjectTool(View* view);

		/// 拾取对象回调（PickResult 包含 shape、worldPoint、object）
		std::function<void(const PickResult&)> OnPicked;

		/// 取消回调
		std::function<void()> OnCancelled;

		/// 设置选择过滤器（可选，仅拾取符合条件的外形）
		void SetFilter(std::shared_ptr<SelectionFilter> filter);

		void MousePress(QMouseEvent* e) override;
		void KeyPress(QKeyEvent* e) override;

	private:
		View* m_view{};
		std::shared_ptr<SelectionFilter> m_filter;
	};

} // namespace SongYun
