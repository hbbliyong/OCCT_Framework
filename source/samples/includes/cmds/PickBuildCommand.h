#pragma once

#include "command/ICommand.h"
#include "command/CommandMacros.h"

namespace Samples
{
	class PickBuildCommand : public SongYun::ICommand
	{
	public:
		bool execute() override;
		QString name() const override { return "PickBuild"; }
	};

} // namespace Samples
