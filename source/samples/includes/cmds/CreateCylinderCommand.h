#pragma once

#include "core/CoreMacro.hpp"
#include "command/ICommand.h"
#include "command/CommandMacros.h"
#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <gp_Pnt.hxx>
#include <QString>
#include <functional>
#include "command/InteractiveCommand.h"
namespace Samples
{

	class CreateCylinderCommand : public SongYun::InteractiveCommand
	{
	public:
		CreateCylinderCommand() = default;

		bool execute() override;
		bool undo() override;
		QString name() const override { return "CreateCylinder"; }

		bool requiresPointPick() const override { return true; }
		void onPickedPoint(const gp_Pnt& point) override;

	private:
		Handle(AIS_InteractiveContext) myContext;
		std::function<void(const QString&)> myStatusCallback;
		Handle(AIS_Shape) myCylinder;
		gp_Pnt myPickedPoint;
		bool myExecuted = false;
	};
	// 自动注册到系统（无需修改initPartModule）
	REGISTER_COMMAND(CreateCylinderCommand, "Samples.CreateCylinder");
} // namespace Samples
