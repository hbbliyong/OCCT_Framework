#pragma once

#include <QEvent>
#include <gp_Pnt.hxx>
#include "selection/PickResult.h"

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
    virtual ~ICommand() = default;

    virtual void Start() = 0;
    virtual void Undo() = 0;
    virtual void Cancel() = 0;
    virtual CommandState State() const = 0;

    virtual bool requiresPointPick() const { return false; }
    virtual bool requiresObjectPick() const { return false; }

    virtual void onPickedPoint(const gp_Pnt &point) { OnPickPoint(point); }
    virtual void onPickedObject(const PickResult &result) {}

    virtual void OnPickPoint(const gp_Pnt &point) {}
    virtual void OnFinish() {}
};
