#pragma once

#include "core/CoreMacro.hpp"
#include <QString>

namespace SongYun {
	class CommandContext;

	/// @brief 命令基类 — 只负责 execute()，undo/redo 由 CommandManager 统一管理
	class ICommand
	{
	public:
		virtual SONGYUN_API ~ICommand() = default;

		/// 执行命令。首次调用做交互+构建，后续调用（redo）幂等恢复显示
		virtual SONGYUN_API bool execute() = 0;

		virtual SONGYUN_API QString name() const = 0;

		void setContext(CommandContext* ctx) { m_context = ctx; }

	protected:
		CommandContext& context() const { return *m_context; }

	private:
		CommandContext* m_context = nullptr;
	};

} // namespace SongYun
