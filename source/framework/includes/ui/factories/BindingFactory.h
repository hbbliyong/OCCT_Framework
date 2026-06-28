#pragma once

#include "core/CoreMacro.hpp"
#include "ui/command_ui/UIItems.h"
#include <QWidget>
#include <vector>
#include <QObject>

namespace SongYun {

	class BindingFactory
	{
	public:
		SONGYUN_API void bind(QWidget* widget, UIItemBase* item);
		SONGYUN_API void unbindAll();

	private:
		std::vector<QMetaObject::Connection> m_connections;
	};

} // namespace SongYun
