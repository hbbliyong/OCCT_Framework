#include "app/App.h"
#include <QApplication>
#include <QSurfaceFormat>

#include "command/CommandManager.h"
#include "tool/ToolManager.h"
#include "document/DocumentManager.h"
#include "selection/SelectionManager.h"
#include "view/ViewManager.h"
namespace SongYun {

	App& App::Instance()
	{
		static App instance;
		return instance;
	}

	CommandContext& App::commandContext()
	{
		return m_commandContext;
	}

	App::App() = default;
	App::~App() = default;

	bool App::initialize(QApplication& app, const QString& appName)
	{
		if (initialized_)
			return false;

		application_ = &app;
		applicationName_ = appName;
		initialized_ = true;

		m_commandManager = std::make_unique<CommandManager>();
		m_toolManager = std::make_unique<ToolManager>();

		m_selectionManager = std::make_unique<SelectionManager>();
		m_viewManager = std::make_unique<ViewManager>();

		m_commandContext.setDocumentManager(
			&DocumentManager::Instance());


		//m_selectionManager->SetView(m_viewManager->activeView());

		m_commandContext.setViewManager(
			m_viewManager.get());

		m_commandContext.setToolManager(
			m_toolManager.get());

		m_commandContext.setSelectionManager(
			m_selectionManager.get());

		return true;
	}

	void App::shutdown()
	{
		initialized_ = false;
		application_ = nullptr;
		applicationName_.clear();
	}

	bool App::isInitialized() const noexcept
	{
		return initialized_;
	}

	QApplication* App::application() const noexcept
	{
		return application_;
	}

	const QString& App::applicationName() const noexcept
	{
		return applicationName_;
	}
	CommandManager& App::commandManager()
	{
		return *m_commandManager;
	}

	//ToolManager& App::toolManager()
	//{
	//	return *m_toolManager;
	//}

	//DocumentManager& App::documentManager()
	//{
	//	return DocumentManager::Instance();
	//}

	//SelectionManager& App::selectionManager()
	//{
	//	return *m_selectionManager;
	//}

	//ViewManager& App::viewManager()
	//{
	//	return *m_viewManager;
	//}
} // namespace SongYun
