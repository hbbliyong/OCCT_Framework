#pragma once

#include "core/CoreMacro.hpp"
#include "ui/command_ui/CommandUI.h"
#include <QString>

namespace SongYun {
	class CommandContext;

	/// @brief 命令基类 — 只负责 execute()，undo/redo 由 CommandManager 统一管理
	class ICommand
	{
	public:
		virtual SONGYUN_API ~ICommand() = default;

		virtual SONGYUN_API bool execute() = 0;
		virtual SONGYUN_API QString name() const = 0;

		/// 命令声明工具栏参数（默认空，子类可 override）
		virtual SONGYUN_API void buildUI(CommandUI& ui) {}

		void setContext(CommandContext* ctx) { m_context = ctx; }

	protected:
		CommandContext& context() const { return *m_context; }

	private:
		CommandContext* m_context = nullptr;
	};

} // namespace SongYun
