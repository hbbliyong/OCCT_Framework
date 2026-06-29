#include "selection/SelectionManager.h"
#include <QEventLoop>
#include "tool/tools/PickPointTool.h"
#include "tool/tools/PickPointRubberTool.h"
#include "tool/tools/PickObjectTool.h"
#include "tool/ToolManager.h"
#include "command/CommandManager.h"
#include "common/StatusService.h"
#include "app/App.h"

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
			// 非抛异常：安全返回 nullopt（上一轮可能未正确清理）
			m_state = SelectionState::Idle;
			return std::nullopt;
		}

		if (!m_view)
			return std::nullopt;

		m_state = SelectionState::PickingPoint;

		App::Instance().statusService().showMessage(prompt);

		QEventLoop loop;
		std::optional<gp_Pnt> result;

		auto tool = std::make_shared<PickPointTool>(m_view);

		tool->OnPicked = [&](const gp_Pnt& pt) { result = pt; loop.quit(); };
		tool->OnCancelled = [&]() { result.reset(); loop.quit(); };

		ToolManager::Instance().SetCurrent(tool);

		App::Instance().commandManager().setActiveLoop(&loop);
		loop.exec();

		ToolManager::Instance().SetCurrent(nullptr);
		m_state = SelectionState::Idle;

		return result;
	}

	std::optional<gp_Pnt>
		SelectionManager::PickPointWithRubber(
			QString prompt,
			const std::optional<gp_Pnt>& anchor)
	{
		if (m_state != SelectionState::Idle)
		{
			m_state = SelectionState::Idle;
			return std::nullopt;
		}

		if (!m_view)
			return std::nullopt;

		m_state = SelectionState::PickingPointWithRubber;

		App::Instance().statusService().showMessage(prompt);

		QEventLoop loop;
		std::optional<gp_Pnt> result;

		auto tool = std::make_shared<PickPointRubberTool>(m_view);

		if (anchor.has_value())
			tool->SetAnchorPoint(anchor.value());

		tool->OnPicked = [&](const gp_Pnt& pt) { result = pt; loop.quit(); };
		tool->OnCancelled = [&]() { result.reset(); loop.quit(); };

		ToolManager::Instance().SetCurrent(tool);

		App::Instance().commandManager().setActiveLoop(&loop);
		loop.exec();

		ToolManager::Instance().SetCurrent(nullptr);
		m_state = SelectionState::Idle;

		return result;
	}

	std::optional<PickResult>
		SelectionManager::PickObject(
			QString prompt,
			std::shared_ptr<SelectionFilter> filter)
	{
		if (m_state != SelectionState::Idle)
		{
			m_state = SelectionState::Idle;
			return std::nullopt;
		}

		if (!m_view)
			return std::nullopt;

		m_state = SelectionState::PickingObject;

		App::Instance().statusService().showMessage(prompt);

		QEventLoop loop;
		std::optional<PickResult> result;

		auto tool = std::make_shared<PickObjectTool>(m_view);

		if (filter)
			tool->SetFilter(filter);

		tool->OnPicked = [&](const PickResult& pr) { result = pr; loop.quit(); };
		tool->OnCancelled = [&]() { result.reset(); loop.quit(); };

		ToolManager::Instance().SetCurrent(tool);

		App::Instance().commandManager().setActiveLoop(&loop);
		loop.exec();

		ToolManager::Instance().SetCurrent(nullptr);
		m_state = SelectionState::Idle;

		return result;
	}
}