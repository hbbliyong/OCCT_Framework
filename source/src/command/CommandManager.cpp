#include "command/CommandManager.h"

void CommandManager::execute(const std::shared_ptr<Command> &command)
{
    if (!command)
        return;

    if (index_ < history_.size())
        history_.erase(history_.begin() + index_, history_.end());

    if (command->execute())
    {
        history_.push_back(command);
        index_ = history_.size();
    }
}

void CommandManager::undo()
{
    if (!canUndo())
        return;

    auto &command = history_[index_ - 1];
    if (command && command->undo())
    {
        --index_;
    }
}

void CommandManager::redo()
{
    if (!canRedo())
        return;

    auto &command = history_[index_];
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
