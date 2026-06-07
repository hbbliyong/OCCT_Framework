#include "command/CommandFactory.h"
namespace SongYun {
	CommandFactory& CommandFactory::instance()
	{
		static CommandFactory factory;
		return factory;
	}

	void CommandFactory::registerCommand(const QString& name, const Creator& creator)
	{
		creators_[name] = creator;
	}

	std::shared_ptr<Command> CommandFactory::create(const QString& name, const std::map<QString, std::any>& params) const
	{
		auto it = creators_.find(name);
		if (it != creators_.end())
		{
			return it->second(params);
		}
		return nullptr;
	}

	bool CommandFactory::contains(const QString& name) const
	{
		return creators_.find(name) != creators_.end();
	}
}