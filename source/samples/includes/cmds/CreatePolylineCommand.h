#pragma once

#include "command/ICommand.h"
#include "command/CommandMacros.h"

namespace Samples
{
	class CreatePolylineCommand : public SongYun::ICommand
	{
	public:
		bool execute() override;
		QString name() const override { return "CreatePolyline"; }
	};

} // namespace Samples
