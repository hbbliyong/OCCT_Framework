#pragma once

#include <QString>
#include <memory>
#include <functional>

class Command
{
public:
    virtual ~Command() = default;

    virtual bool execute() = 0;
    virtual bool undo() { return false; }
    virtual bool redo() { return false; }
    virtual QString name() const = 0;
};
