#pragma once

#include "core/CoreMacro.hpp"
#include "ui/Ribbon/RibbonGroupWidget.h"
#include <QWidget>
#include <QHBoxLayout>
#include <QMap>
#include <QString>

namespace SongYun {

	class SONGYUN_API RibbonPageWidget : public QWidget
	{
		Q_OBJECT
	public:
		explicit RibbonPageWidget(QWidget* parent = nullptr)
			: QWidget(parent)
		{
			m_layout = new QHBoxLayout(this);
			m_layout->setContentsMargins(4, 2, 4, 2);
			m_layout->setSpacing(4);
		}

		RibbonGroupWidget* getOrCreateGroup(const QString& name)
		{
			if (m_groups.contains(name))
				return m_groups[name];
			auto* group = new RibbonGroupWidget(name, this);
			m_layout->addWidget(group);
			m_groups[name] = group;
			return group;
		}

		QHBoxLayout* layout() { return m_layout; }
		QHBoxLayout* m_layout;
		QMap<QString, RibbonGroupWidget*> m_groups;
	};

} // namespace SongYun
