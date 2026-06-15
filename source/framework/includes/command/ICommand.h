#pragma once

#include "core/CoreMacro.hpp"
#include <QString>
#include <memory>
#include <functional>

namespace SongYun {
	class CommandContext;
	class ICommand
	{
	public:
		virtual SONGYUN_API ~ICommand() = default;

		virtual SONGYUN_API bool execute() = 0;
		virtual SONGYUN_API bool undo() { return false; }
		virtual SONGYUN_API bool redo() { return false; }
		virtual SONGYUN_API QString name() const = 0;
		void setContext(CommandContext* ctx)
		{
			m_context = ctx;
		}
	protected:

		CommandContext& context() const
		{
			return *m_context;
		}
	private:

		CommandContext* m_context = nullptr;
	};

} // namespace SongYun
