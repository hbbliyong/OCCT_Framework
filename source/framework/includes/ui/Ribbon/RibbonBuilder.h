#pragma once

#include "core/CoreMacro.hpp"
#include "ui/ribbon/RibbonModel.h"

namespace SongYun {

	class RibbonWidget;
	class ActionManager;

	class RibbonBuilder
	{
	public:
		SONGYUN_API void build(RibbonWidget* ribbon, const RibbonModel& model,
							   ActionManager& am);
	};

} // namespace SongYun
