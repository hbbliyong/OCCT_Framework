#pragma once

#include "core/CoreMacro.hpp"
#include "command/ICommand.h"
#include <memory>
#include <vector>

namespace SongYun {

	class CommandManager
	{
	public:
		SONGYUN_API CommandManager() = default;

		SONGYUN_API void execute(const std::shared_ptr<ICommand>& command);
		SONGYUN_API void undo();
		SONGYUN_API void redo();
		SONGYUN_API void clear();

		SONGYUN_API bool canUndo() const;
		SONGYUN_API bool canRedo() const;

	private:
		std::vector<std::shared_ptr<ICommand>> history_;
		size_t index_ = 0;
	};

} // namespace SongYun
