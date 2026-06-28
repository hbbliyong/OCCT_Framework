#pragma once

#include "command/ICommand.h"
#include "command/CommandMacros.h"

namespace Samples
{
	class DrawArcCommand : public SongYun::ICommand
	{
	public:
		bool execute() override;
		QString name() const override { return "DrawArc"; }
	};

} // namespace Samples
