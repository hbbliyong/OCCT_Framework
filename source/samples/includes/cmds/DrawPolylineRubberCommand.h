#pragma once

#include "command/ICommand.h"
#include "command/CommandMacros.h"

namespace Samples
{
	class DrawPolylineRubberCommand : public SongYun::ICommand
	{
		struct Options { bool closed = false; };

	public:
		bool execute() override;
		void buildUI(SongYun::CommandUI& ui) override;
		QString name() const override { return "DrawPolylineRubber"; }

	private:
		Options m_opts;
	};

} // namespace Samples
