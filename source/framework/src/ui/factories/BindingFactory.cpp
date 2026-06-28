#include "ui/factories/BindingFactory.h"
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QComboBox>

namespace SongYun {

	void BindingFactory::bind(QWidget* widget, UIItemBase* item)
	{
		if (!widget || !item) return;

		if (auto* b = dynamic_cast<BoolItem*>(item))
		{
			auto* cb = qobject_cast<QCheckBox*>(widget);
			if (!cb || !b->setter) return;
			auto conn = QObject::connect(cb, &QCheckBox::toggled, b->setter);
			m_connections.push_back(conn);
			return;
		}
		if (auto* d = dynamic_cast<DoubleItem*>(item))
		{
			auto* sp = qobject_cast<QDoubleSpinBox*>(widget);
			if (!sp || !d->setter) return;
			auto conn = QObject::connect(sp, QOverload<double>::of(&QDoubleSpinBox::valueChanged), d->setter);
			m_connections.push_back(conn);
			return;
		}
		if (auto* e = dynamic_cast<EnumItem*>(item))
		{
			auto* cbo = qobject_cast<QComboBox*>(widget);
			if (!cbo || !e->setter) return;
			auto conn = QObject::connect(cbo, &QComboBox::currentTextChanged, e->setter);
			m_connections.push_back(conn);
			return;
		}
	}

	void BindingFactory::unbindAll()
	{
		for (auto& conn : m_connections) QObject::disconnect(conn);
		m_connections.clear();
	}

} // namespace SongYun
