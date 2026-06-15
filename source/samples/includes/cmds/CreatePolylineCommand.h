#pragma once

#include "core/CoreMacro.hpp"
#include "command/InteractiveCommand.h"
#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <gp_Pnt.hxx>
#include <QString>
#include <functional>
#include <vector>
#include "command/CommandMacros.h"
namespace Samples {

	class CreatePolylineCommand : public SongYun::InteractiveCommand
	{
	public:
		CreatePolylineCommand() = default;

		bool execute() override;
		bool undo() override;
		QString name() const override { return "CreatePolyline"; }

		bool requiresPointPick() const override { return true; }
		void onPickedPoint(const gp_Pnt& point) override;
		void onFinish() override;

	private:
		void cleanupMarkers();
		void buildPolyline();

	private:
		Handle(AIS_InteractiveContext) myContext;
		std::function<void(const QString&)> myStatusCallback;
		std::vector<gp_Pnt> myPoints;
		std::vector<Handle(AIS_Shape)> myMarkers;
		Handle(AIS_Shape) myPolyline;
		bool myExecuted = false;
	};
	// 自动注册到系统（无需修改initPartModule）

} // namespace Samples
	// \source\framework\includes\command\commands\CreatePolylineCommand.cpp