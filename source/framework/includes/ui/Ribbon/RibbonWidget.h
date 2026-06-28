#pragma once

#include "core/CoreMacro.hpp"
#include <QTabWidget>
#include <QMap>
#include <QString>

namespace SongYun {

	class RibbonPageWidget;

	class SONGYUN_API RibbonWidget : public QTabWidget
	{
		Q_OBJECT
	public:
		explicit RibbonWidget(QWidget* parent = nullptr);

		RibbonPageWidget* addPage(const QString& name);

	private:
		QMap<QString, RibbonPageWidget*> m_pages;
	};

} // namespace SongYun
