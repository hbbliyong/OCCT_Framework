#include "command/ActionManager.h"
#include "command/CommandManager.h"
#include "command/CommandRegistry.h"
#include "command/ICommand.h"

#include <QAction>

namespace SongYun {

	ActionManager::ActionManager(CommandManager& cmdMgr) : m_cmdMgr(cmdMgr) {}

	QAction* ActionManager::createCommandAction(const CommandActionConfig& cfg)
	{
		if (!cfg.id) return nullptr;

		// 缓存命中
		auto it = m_cache.find(cfg.id);
		if (it != m_cache.end())
			return it->second;

		// 文本：优先用配置的 text，否则取命令 name()
		QString text = cfg.text;
		if (text.isEmpty())
		{
			auto* cmd = CommandRegistry::Instance().getCommand(cfg.id);
			text = cmd ? cmd->name() : cfg.id;
		}

		auto* action = new QAction(text);
		action->setCheckable(false);

		if (!cfg.icon.isEmpty())
			action->setIcon(QIcon(cfg.icon));
		if (!cfg.shortcut.isEmpty())
			action->setShortcut(QKeySequence(cfg.shortcut));

		QObject::connect(action, &QAction::triggered, [this, id = std::string(cfg.id)]()
		{
			m_cmdMgr.execute(id);
		});

		m_cache[cfg.id] = action;
		return action;
	}

} // namespace SongYun
