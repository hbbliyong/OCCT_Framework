#include "selection/SelectionManager.h"
#include <QEventLoop>
#include "tool/tools/PickPointTool.h"
#include "tool/ToolManager.h"
#include <stdexcept>

namespace SongYun
{
	void SelectionManager::setFilter(std::shared_ptr<SelectionFilter> filter)
	{
		filter_ = std::move(filter);
	}

	std::shared_ptr<SelectionFilter> SelectionManager::filter() const
	{
		return filter_;
	}

	void SelectionManager::setCurrentSelection(const PickResult& result)
	{
		currentSelection_ = result;
	}

	const PickResult& SelectionManager::currentSelection() const
	{
		return currentSelection_;
	}
	std::optional<gp_Pnt>
		SelectionManager::PickPoint(
			QString prompt)
	{
		if (m_state != SelectionState::Idle)
		{
			throw std::runtime_error(
				"Selection already active.");
		}

		if (!m_view)
		{
			throw std::runtime_error(
				"No active view.");
		}

		m_state =
			SelectionState::PickingPoint;

		QEventLoop loop;

		std::optional<gp_Pnt> result;

		auto tool =
			std::make_shared<
			PickPointTool>(m_view);

		tool->OnPicked =
			[&](const gp_Pnt& pt)
			{
				result = pt;
				loop.quit();
			};

		tool->OnCancelled =
			[&]()
			{
				result.reset();
				loop.quit();
			};

		ToolManager::Instance()
			.SetCurrent(tool);

		// MainWindow状态栏显示
		// StatusBar::Show(prompt);

		loop.exec();

		ToolManager::Instance()
			.SetCurrent(nullptr);

		m_state =
			SelectionState::Idle;

		return result;
	}
}