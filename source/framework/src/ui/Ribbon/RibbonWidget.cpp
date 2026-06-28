#include "ui/Ribbon/RibbonWidget.h"
#include "ui/Ribbon/RibbonPageWidget.h"

namespace SongYun
{
	RibbonWidget::RibbonWidget(QWidget* parent)
		: QTabWidget(parent)
	{
		setTabPosition(QTabWidget::North);
		setDocumentMode(true);
	}

	RibbonPageWidget* RibbonWidget::addPage(const QString& name)
	{
		auto* page = new RibbonPageWidget(this);
		addTab(page, name);
		m_pages[name] = page;
		return page;
	}

} // namespace SongYun
