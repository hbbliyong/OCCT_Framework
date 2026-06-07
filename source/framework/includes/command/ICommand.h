#pragma once

#include "core/CoreMacro.hpp"
#include <QEvent>
#include <gp_Pnt.hxx>
#include "selection/PickResult.h"

namespace SongYun {

enum class CommandState
{
    Idle,
    Running,
    Completed,
    Cancelled,
    Failed
};

class ICommand
{
public:
    virtual SONGYUN_API ~ICommand() = default;

    virtual SONGYUN_API void Start() = 0;
    virtual SONGYUN_API void Undo() = 0;
    virtual SONGYUN_API void Cancel() = 0;
    virtual SONGYUN_API CommandState State() const = 0;

    virtual SONGYUN_API bool requiresPointPick() const { return false; }
    virtual SONGYUN_API bool requiresObjectPick() const { return false; }

    virtual SONGYUN_API void onPickedPoint(const gp_Pnt &point) { OnPickPoint(point); }
    virtual SONGYUN_API void onPickedObject(const PickResult &result) {}

    virtual SONGYUN_API void OnPickPoint(const gp_Pnt &point) {}
    virtual SONGYUN_API void OnFinish() {}
};

} // namespace SongYun
