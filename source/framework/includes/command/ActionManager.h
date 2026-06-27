#pragma once

#include "core/CoreMacro.hpp"
#include <QString>
#include <memory>
#include <unordered_map>
#include <string>

class QAction;
class QKeySequence;

namespace SongYun {

	class CommandManager;

	/// @brief 命令 Action 配置
	struct CommandActionConfig
	{
		const char* id       = nullptr;   // 命令 ID（必填）
		QString     text;                 // 按钮文本（为空则用命令 name()）
		QString     icon;                 // 图标路径（可选）
		QString     shortcut;             // 快捷键（可选，如 "Ctrl+N"）
	};

	/// @brief Action 管理器 — 缓存 + 复用，构造时注入 CommandManager
	class ActionManager
	{
	public:
		SONGYUN_API explicit ActionManager(CommandManager& cmdMgr);

		/// 创建（或从缓存获取）命令 Action。同一 commandId 在菜单/工具栏复用
		SONGYUN_API QAction* createCommandAction(const CommandActionConfig& cfg);

	private:
		CommandManager& m_cmdMgr;
		std::unordered_map<std::string, QAction*> m_cache;
	};

} // namespace SongYun
