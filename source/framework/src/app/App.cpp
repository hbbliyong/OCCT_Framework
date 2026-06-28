#include "app/App.h"
#include "ui/MainWindow/MainWindow.h"
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
		if (m_initialized) return false;

		m_application = &app;
		m_appName     = appName;
		m_initialized = true;

		m_eventBus         = std::make_unique<EventBus>();
		m_commandManager   = std::make_unique<CommandManager>();
		m_toolManager      = std::make_unique<ToolManager>();
		m_selectionManager = std::make_unique<SelectionManager>();
		m_viewManager      = std::make_unique<ViewManager>();
		m_statusService    = std::make_unique<StatusService>(m_eventBus.get());

		m_commandContext.setDocumentManager(&DocumentManager::Instance());
		m_commandContext.setViewManager(m_viewManager.get());
		m_commandContext.setToolManager(m_toolManager.get());
		m_commandContext.setSelectionManager(m_selectionManager.get());
		m_commandContext.setStatusService(m_statusService.get());

		return true;
	}

	void App::shutdown()
	{
		m_mainWindow.reset();    // 先销毁 MainWindow
		m_initialized = false;
		m_application = nullptr;
		m_appName.clear();
	}

	bool App::isInitialized() const noexcept          { return m_initialized; }
	QApplication* App::application() const noexcept   { return m_application; }
	const QString& App::applicationName() const noexcept { return m_appName; }
	CommandManager& App::commandManager()             { return *m_commandManager; }
	EventBus& App::eventBus()                         { return *m_eventBus; }
	StatusService& App::statusService()               { return *m_statusService; }
	MainWindow& App::mainWindow(Document* doc)
	{
		if (!m_mainWindow)
			m_mainWindow = std::make_unique<MainWindow>(doc);
		return *m_mainWindow;
	}

} // namespace SongYun
