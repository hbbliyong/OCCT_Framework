#pragma once

#include "core/CoreMacro.hpp"
#include "ICommand.h"
#include <QString>
#include <map>
#include <functional>
#include <memory>
#include <any>

namespace SongYun {

	class CommandFactory
	{
	public:
		using Creator = std::function<std::shared_ptr<ICommand>(const std::map<QString, std::any>&)>;

		SONGYUN_API static CommandFactory& instance();

		SONGYUN_API void registerCommand(const QString& name, const Creator& creator);
		SONGYUN_API std::shared_ptr<ICommand> create(const QString& name, const std::map<QString, std::any>& params = {}) const;
		SONGYUN_API bool contains(const QString& name) const;

	private:
		CommandFactory() = default;
		std::map<QString, Creator> creators_;
	};

} // namespace SongYun
