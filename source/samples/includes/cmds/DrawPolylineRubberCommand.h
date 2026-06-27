#pragma once

#include "command/ICommand.h"
#include "command/CommandMacros.h"

namespace Samples
{
	class DrawPolylineRubberCommand : public SongYun::ICommand
	{
	public:
		bool execute() override;
		QString name() const override { return "DrawPolylineRubber"; }
	};

} // namespace Samples
