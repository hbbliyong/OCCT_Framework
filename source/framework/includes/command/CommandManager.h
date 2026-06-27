#pragma once

#include "core/CoreMacro.hpp"
#include "command/ICommand.h"
#include "command/Transaction.h"
#include "common/Singleton.h"
#include <memory>
#include <vector>

class QEventLoop;

namespace SongYun {

	class Document;

	/// @brief 命令管理器 — Transaction 生命周期 + undo/redo + 中断
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

	private:
		Document* activeDocument() const;

		std::vector<std::unique_ptr<Transaction>> m_undoStack;
		std::vector<std::unique_ptr<Transaction>> m_redoStack;

		ICommand* m_activeCommand = nullptr;
		QEventLoop* m_activeLoop = nullptr;
	};

} // namespace SongYun
