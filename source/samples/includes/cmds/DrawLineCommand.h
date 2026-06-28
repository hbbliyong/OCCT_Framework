#pragma once

#include "command/ICommand.h"
#include "command/CommandMacros.h"

namespace Samples
{
	class DrawLineCommand : public SongYun::ICommand
	{
	public:
		bool execute() override;
		QString name() const override { return "DrawLine"; }
	};

} // namespace Samples
