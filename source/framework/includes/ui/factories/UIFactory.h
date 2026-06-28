#pragma once

#include "core/CoreMacro.hpp"
#include "ui/command_ui/UIItems.h"
#include <QWidget>

namespace SongYun {

	class UIFactory
	{
	public:
		SONGYUN_API QWidget* create(UIItemBase* item, QWidget* parent = nullptr);
	};

} // namespace SongYun
