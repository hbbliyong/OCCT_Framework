#pragma once

#include "core/CoreMacro.hpp"
#include <QString>
#include "command/CommandContext.h"

class QApplication;
namespace SongYun {

	class CommandManager;
	class ToolManager;
	class DocumentManager;
	class SelectionManager;
	class ViewManager;
	class EventBus;
	class StatusService;

	class SONGYUN_API App
	{
	public:
		static App& Instance();

		bool initialize(QApplication& app, const QString& appName = "VtkQtMinimal");
		void shutdown();
		bool isInitialized() const noexcept;
		QApplication* application() const noexcept;
		const QString& applicationName() const noexcept;

		CommandManager& commandManager();
		CommandContext& commandContext();
		EventBus&       eventBus();
		StatusService&  statusService();

	private:
		App();
		~App();
		App(const App&) = delete;
		App& operator=(const App&) = delete;

		QApplication* application_ = nullptr;
		QString applicationName_;
		bool initialized_ = false;

		std::unique_ptr<EventBus>        m_eventBus;
		std::unique_ptr<CommandManager>  m_commandManager;
		std::unique_ptr<ToolManager>     m_toolManager;
		std::unique_ptr<SelectionManager> m_selectionManager;
		std::unique_ptr<ViewManager>     m_viewManager;
		std::unique_ptr<StatusService>   m_statusService;
		CommandContext m_commandContext;
	};

} // namespace SongYun
