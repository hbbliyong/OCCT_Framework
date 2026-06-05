#pragma once

#include "command/Command.h"
#include "selection/PickResult.h"
#include <gp_Pnt.hxx>

class InteractiveCommand : public Command
{
public:
    virtual ~InteractiveCommand() = default;

    virtual bool requiresPointPick() const { return false; }
    virtual bool requiresObjectPick() const { return false; }

    virtual void onPickedPoint(const gp_Pnt &point) {}
    virtual void onPickedObject(const PickResult &result) {}
    virtual void onFinish() {}
};
