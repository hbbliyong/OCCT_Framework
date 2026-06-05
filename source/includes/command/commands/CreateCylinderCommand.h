#pragma once

#include "command/InteractiveCommand.h"
#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <gp_Pnt.hxx>
#include <QString>
#include <functional>

class CreateCylinderCommand : public InteractiveCommand
{
public:
    CreateCylinderCommand(const Handle(AIS_InteractiveContext) & context,
                          std::function<void(const QString &)> statusCallback = nullptr);

    bool execute() override;
    bool undo() override;
    QString name() const override { return "CreateCylinder"; }

    bool requiresPointPick() const override { return true; }
    void onPickedPoint(const gp_Pnt &point) override;

private:
    Handle(AIS_InteractiveContext) myContext;
    std::function<void(const QString &)> myStatusCallback;
    Handle(AIS_Shape) myCylinder;
    gp_Pnt myPickedPoint;
    bool myExecuted = false;
};
