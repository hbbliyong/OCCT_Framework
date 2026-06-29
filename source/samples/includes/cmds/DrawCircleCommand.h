#pragma once

#include "command/ICommand.h"
#include "command/CommandMacros.h"

namespace Samples
{
	class DrawCircleCommand : public SongYun::ICommand
	{
		enum Mode { CenterRadius, TwoPoints, ThreePoints };

		struct Options {
			int mode = CenterRadius;
		};

	public:
		bool execute() override;
		void buildUI(SongYun::CommandUI& ui) override;
		QString name() const override { return "DrawCircle"; }

	private:
		bool executeCenterRadius();
		bool executeTwoPoints();
		bool executeThreePoints();

		Options m_opts;
	};

} // namespace Samples
