#pragma once

#include "core/CoreMacro.hpp"
#include <QString>
#include <memory>
#include <functional>

namespace SongYun {

class Command
{
public:
    virtual SONGYUN_API ~Command() = default;

    virtual SONGYUN_API bool execute() = 0;
    virtual SONGYUN_API bool undo() { return false; }
    virtual SONGYUN_API bool redo() { return false; }
    virtual SONGYUN_API QString name() const = 0;
};

} // namespace SongYun
