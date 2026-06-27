#pragma once

#include "core/CoreMacro.hpp"
#include "common/Singleton.h"
#include "selection/PickResult.h"
#include <memory>
#include <optional>
#include <qstring.h>
namespace SongYun
{

	class SelectionFilter;
	class View;

	enum class SelectionState
	{
		Idle,
		PickingPoint,
		PickingPointWithRubber,
		PickingObject
	};

	class SelectionManager
	{
	public:
		SONGYUN_API void setFilter(std::shared_ptr<SelectionFilter> filter);
		SONGYUN_API std::shared_ptr<SelectionFilter> filter() const;

		SONGYUN_API void setCurrentSelection(const PickResult& result);
		SONGYUN_API const PickResult& currentSelection() const;

		void SetView(
			View* view) {
			m_view = view;
		}

		/// 拾取一个点（无预览线）
		SONGYUN_API std::optional<gp_Pnt>
			PickPoint(
				QString prompt);

		/// 拾取一个点（带橡皮线，从 anchor 到鼠标实时显示预览线）
		/// 首次调用可传 std::nullopt 作为anchor，后续调用传入上一个点
		SONGYUN_API std::optional<gp_Pnt>
			PickPointWithRubber(
				QString prompt,
				const std::optional<gp_Pnt>& anchor = std::nullopt);

		/// 拾取场景中的一个对象
		/// filter 可选，仅拾取符合条件的形状
		SONGYUN_API std::optional<PickResult>
			PickObject(
				QString prompt,
				std::shared_ptr<SelectionFilter> filter = nullptr);

	private:
		std::shared_ptr<SelectionFilter> filter_;
		PickResult currentSelection_;

		View* m_view{};

		SelectionState m_state =
			SelectionState::Idle;
	};

} // namespace SongYun
