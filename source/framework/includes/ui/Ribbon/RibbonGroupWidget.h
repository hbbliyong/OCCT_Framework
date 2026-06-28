#pragma once

#include "core/CoreMacro.hpp"
#include <QGroupBox>
#include <QHBoxLayout>

namespace SongYun {

	class SONGYUN_API RibbonGroupWidget : public QGroupBox
	{
		Q_OBJECT
	public:
		explicit RibbonGroupWidget(const QString& title, QWidget* parent = nullptr)
			: QGroupBox(title, parent)
		{
			m_layout = new QHBoxLayout(this);
			m_layout->setContentsMargins(4, 8, 8, 4);
			m_layout->setSpacing(6);
			setFlat(false);
			setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
		}

		QHBoxLayout* contentLayout() const { return m_layout; }

	private:
		QHBoxLayout* m_layout;
	};

} // namespace SongYun
