#pragma once

#include "core/CoreMacro.hpp"
#include "ui/command_ui/CommandUI.h"
#include "ui/factories/UIFactory.h"
#include "ui/factories/BindingFactory.h"
#include <QWidget>

namespace SongYun {

	class ToolbarBuilder
	{
	public:
		SONGYUN_API QWidget* build(const CommandUI& ui, QWidget* parent = nullptr);
		SONGYUN_API void clear();

	private:
		UIFactory m_factory;
		BindingFactory m_binding;
	};

} // namespace SongYun
