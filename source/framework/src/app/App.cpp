#include "app/App.h"
#include <QApplication>
#include <QSurfaceFormat>

#include "command/CommandManager.h"
#include "tool/ToolManager.h"
#include "document/DocumentManager.h"
#include "selection/SelectionManager.h"
#include "view/ViewManager.h"
#include "common/EventBus.h"
#include "common/StatusService.h"

namespace SongYun {

	App& App::Instance() { static App instance; return instance; }

	CommandContext& App::commandContext() { return m_commandContext; }

	App::App() = default;
	App::~App() = default;

	bool App::initialize(QApplication& app, const QString& appName)
	{
		if (initialized_) return false;

		application_ = &app;
		applicationName_ = appName;
		initialized_ = true;

		m_eventBus        = std::make_unique<EventBus>();
		m_commandManager  = std::make_unique<CommandManager>();
		m_toolManager     = std::make_unique<ToolManager>();
		m_selectionManager= std::make_unique<SelectionManager>();
		m_viewManager     = std::make_unique<ViewManager>();
		m_statusService   = std::make_unique<StatusService>(m_eventBus.get());

		m_commandContext.setDocumentManager(&DocumentManager::Instance());
		m_commandContext.setViewManager(m_viewManager.get());
		m_commandContext.setToolManager(m_toolManager.get());
		m_commandContext.setSelectionManager(m_selectionManager.get());
		m_commandContext.setStatusService(m_statusService.get());

		return true;
	}

	void App::shutdown()
	{
		initialized_ = false;
		application_ = nullptr;
		applicationName_.clear();
	}

	bool App::isInitialized() const noexcept         { return initialized_; }
	QApplication* App::application() const noexcept  { return application_; }
	const QString& App::applicationName() const noexcept { return applicationName_; }
	CommandManager& App::commandManager()            { return *m_commandManager; }
	EventBus& App::eventBus()                        { return *m_eventBus; }
	StatusService& App::statusService()              { return *m_statusService; }

} // namespace SongYun
