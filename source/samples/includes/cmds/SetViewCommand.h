#pragma once

#include "command/ICommand.h"
#include "command/CommandMacros.h"

namespace Samples
{
	/// @brief 视图切换命令：3D / XOY / XOZ / YOZ
	class SetViewCommand : public SongYun::ICommand
	{
	public:
		enum Mode { Axo, Top, Front, Left };

		explicit SetViewCommand(Mode mode) : m_mode(mode) {}
		bool execute() override;
		QString name() const override;

	private:
		Mode m_mode;
	};

} // namespace Samples
