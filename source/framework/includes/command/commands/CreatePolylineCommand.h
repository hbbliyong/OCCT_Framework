#pragma once

#include "core/CoreMacro.hpp"
#include "command/InteractiveCommand.h"
#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <gp_Pnt.hxx>
#include <QString>
#include <functional>
#include <vector>

namespace SongYun {

class CreatePolylineCommand : public InteractiveCommand
{
public:
    SONGYUN_API CreatePolylineCommand(const Handle(AIS_InteractiveContext) & context,
                          std::function<void(const QString &)> statusCallback = nullptr);

    SONGYUN_API bool execute() override;
    SONGYUN_API bool undo() override;
    SONGYUN_API QString name() const override { return "CreatePolyline"; }

    SONGYUN_API bool requiresPointPick() const override { return true; }
    SONGYUN_API void onPickedPoint(const gp_Pnt &point) override;
    SONGYUN_API void onFinish() override;

private:
    void cleanupMarkers();
    void buildPolyline();

private:
    Handle(AIS_InteractiveContext) myContext;
    std::function<void(const QString &)> myStatusCallback;
    std::vector<gp_Pnt> myPoints;
    std::vector<Handle(AIS_Shape)> myMarkers;
    Handle(AIS_Shape) myPolyline;
    bool myExecuted = false;
};

} // namespace SongYun
