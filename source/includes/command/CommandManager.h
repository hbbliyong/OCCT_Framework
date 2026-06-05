#pragma once

#include "command/Command.h"
#include <memory>
#include <vector>

class CommandManager
{
public:
    CommandManager() = default;

    void execute(const std::shared_ptr<Command> &command);
    void undo();
    void redo();
    void clear();

    bool canUndo() const;
    bool canRedo() const;

private:
    std::vector<std::shared_ptr<Command>> history_;
    size_t index_ = 0;
};
