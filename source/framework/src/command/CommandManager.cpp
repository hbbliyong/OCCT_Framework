#include "command/CommandManager.h"
#include "command/CommandRegistry.h"
#include "iostream"
#include "app/App.h"
namespace SongYun {

	void CommandManager::execute(const std::shared_ptr<ICommand>& command)
	{
		if (!command)
			return;
		command->setContext(&App::Instance().commandContext());
		if (index_ < history_.size())
			history_.erase(history_.begin() + index_, history_.end());

		if (command->execute())
		{
			history_.push_back(command);
			index_ = history_.size();
		}
	}

	SONGYUN_API void CommandManager::execute(const std::string& commandId)
	{
		const auto command = SongYun::CommandRegistry::Instance().getCommand(commandId.c_str());
		std::cout << "Executing command: " << &SongYun::CommandRegistry::Instance() << std::endl;
		if (command)
		{
			std::shared_ptr<ICommand> cmdPtr(command); // 直接使用原始指针创建shared_ptr

			execute(cmdPtr);
		}
	}

	void CommandManager::undo()
	{
		if (!canUndo())
			return;

		auto& command = history_[index_ - 1];
		if (command && command->undo())
		{
			--index_;
		}
	}

	void CommandManager::redo()
	{
		if (!canRedo())
			return;

		auto& command = history_[index_];
		if (command && command->execute())
		{
			++index_;
		}
	}

	void CommandManager::clear()
	{
		history_.clear();
		index_ = 0;
	}

	bool CommandManager::canUndo() const
	{
		return index_ > 0 && index_ <= history_.size();
	}

	bool CommandManager::canRedo() const
	{
		return index_ < history_.size();
	}

} // namespace SongYun
