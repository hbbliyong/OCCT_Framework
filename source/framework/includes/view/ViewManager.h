#pragma once

#include "core/CoreMacro.hpp"
#include <QObject>

namespace SongYun {

	class View;

	class ViewManager : public QObject
	{
		Q_OBJECT

	public:
		SONGYUN_API explicit ViewManager(QObject* parent = nullptr);

		SONGYUN_API void setActiveView(View* view);
		SONGYUN_API View* activeView() const;

	private:
		View* activeView_ = nullptr;
	};

} // namespace SongYun
