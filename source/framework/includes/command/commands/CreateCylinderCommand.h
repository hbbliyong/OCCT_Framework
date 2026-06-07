#pragma once

#include "core/CoreMacro.hpp"
#include "command/InteractiveCommand.h"
#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <gp_Pnt.hxx>
#include <QString>
#include <functional>

namespace SongYun {

class CreateCylinderCommand : public InteractiveCommand
{
public:
    SONGYUN_API CreateCylinderCommand(const Handle(AIS_InteractiveContext) & context,
                          std::function<void(const QString &)> statusCallback = nullptr);

    SONGYUN_API bool execute() override;
    SONGYUN_API bool undo() override;
    SONGYUN_API QString name() const override { return "CreateCylinder"; }

    SONGYUN_API bool requiresPointPick() const override { return true; }
    SONGYUN_API void onPickedPoint(const gp_Pnt &point) override;

private:
    Handle(AIS_InteractiveContext) myContext;
    std::function<void(const QString &)> myStatusCallback;
    Handle(AIS_Shape) myCylinder;
    gp_Pnt myPickedPoint;
    bool myExecuted = false;
};

} // namespace SongYun
