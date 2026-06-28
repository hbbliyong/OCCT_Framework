#pragma once

#include "command/ICommand.h"
#include "command/CommandMacros.h"

namespace Samples
{
	class CreateCylinderCommand : public SongYun::ICommand
	{
		struct Options {
			double radius = 20.0;
			double height = 120.0;
		};

	public:
		bool execute() override;
		void buildUI(SongYun::CommandUI& ui) override;
		QString name() const override { return "CreateCylinder"; }

	private:
		Options m_opts;
	};

} // namespace Samples
