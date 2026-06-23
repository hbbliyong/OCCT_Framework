#pragma once

#include "core/CoreMacro.hpp"
#include "command/ICommand.h"
#include <memory>
#include <vector>
#include "common/Singleton.h"
namespace SongYun {

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

	private:
		std::vector<ICommand*> history_;
		size_t index_ = 0;
	};

} // namespace SongYun
