#pragma once

#include <QString>
#include <QStringList>
#include <QVariant>
#include <functional>

namespace SongYun {

	class UIItemBase
	{
	public:
		virtual ~UIItemBase() = default;
		QString label;
		QString key;
	};

	class BoolItem : public UIItemBase
	{
	public:
		bool                   defaultValue = false;
		std::function<bool()>       getter;
		std::function<void(bool)>   setter;
	};

	class DoubleItem : public UIItemBase
	{
	public:
		double min = 0, max = 100, step = 1, defaultValue = 0;
		std::function<double()>               getter;
		std::function<void(double)>           setter;
	};

	class EnumItem : public UIItemBase
	{
	public:
		QStringList values;
		int         defaultValue = 0;
		std::function<QString()>              getter;
		std::function<void(const QString&)>   setter;
	};

	class ButtonItem : public UIItemBase
	{
	public:
		std::function<void()> onClick;
	};

} // namespace SongYun
