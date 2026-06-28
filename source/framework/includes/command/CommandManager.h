#pragma once

#include "core/CoreMacro.hpp"
#include "command/ICommand.h"
#include "command/Transaction.h"
#include "ui/toolbar/ToolbarBuilder.h"
#include "common/Singleton.h"
#include <memory>
#include <vector>

class QEventLoop;
class QToolBar;

namespace SongYun {

	class Document;

	class CommandManager : public Singleton<CommandManager>
	{
	public:
		SONGYUN_API void execute(ICommand* command);
		SONGYUN_API void execute(const std::string& commandId);
		SONGYUN_API void undo();
		SONGYUN_API void redo();
		SONGYUN_API void clear();

		SONGYUN_API bool canUndo() const;
		SONGYUN_API bool canRedo() const;

		SONGYUN_API void setActiveLoop(QEventLoop* loop);
		SONGYUN_API void interruptActive();
		SONGYUN_API bool isExecuting() const;

		/// 设置命令参数面板的目标 toolbar（MainWindow 调用）
		SONGYUN_API void setParameterToolBar(QToolBar* toolbar);

	private:
		Document* activeDocument() const;
		void buildCommandUI(ICommand* command);
		void clearCommandUI();

		std::vector<std::unique_ptr<Transaction>> m_undoStack;
		std::vector<std::unique_ptr<Transaction>> m_redoStack;

		ICommand* m_activeCommand = nullptr;
		QEventLoop* m_activeLoop = nullptr;

		QToolBar* m_paramToolbar = nullptr;
		ToolbarBuilder m_builder;
	};

} // namespace SongYun
