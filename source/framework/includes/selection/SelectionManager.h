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
	class OccView;

	enum class SelectionState
	{
		Idle,
		PickingPoint,
		PickingObject
	};

	class SelectionManager : public Singleton<SelectionManager>
	{
	public:
		SONGYUN_API void setFilter(std::shared_ptr<SelectionFilter> filter);
		SONGYUN_API std::shared_ptr<SelectionFilter> filter() const;

		SONGYUN_API void setCurrentSelection(const PickResult& result);
		SONGYUN_API const PickResult& currentSelection() const;

		void SetView(
			OccView* view) {
			m_view = view;
		}

		std::optional<gp_Pnt>
			PickPoint(
				QString prompt);

	private:
		std::shared_ptr<SelectionFilter> filter_;
		PickResult currentSelection_;

		OccView* m_view{};

		SelectionState m_state =
			SelectionState::Idle;
	};

} // namespace SongYun
