#pragma once

#include "core/CoreMacro.hpp"
#include "command/ICommand.h"
#include "selection/PickResult.h"
#include <gp_Pnt.hxx>

namespace SongYun {

	class InteractiveCommand : public ICommand
	{
	public:
		virtual SONGYUN_API ~InteractiveCommand() = default;

		virtual SONGYUN_API bool requiresPointPick() const { return false; }
		virtual SONGYUN_API bool requiresObjectPick() const { return false; }

		virtual SONGYUN_API void onPickedPoint(const gp_Pnt& point) {}
		virtual SONGYUN_API void onPickedObject(const PickResult& result) {}
		virtual SONGYUN_API void onFinish() {}
	};

} // namespace SongYun
