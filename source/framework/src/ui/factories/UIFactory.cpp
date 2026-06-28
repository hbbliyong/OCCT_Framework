#include "ui/factories/UIFactory.h"
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QPushButton>

namespace SongYun {

	QWidget* UIFactory::create(UIItemBase* item, QWidget* parent)
	{
		if (!item) return nullptr;

		if (auto* b = dynamic_cast<BoolItem*>(item))
		{
			auto* w = new QCheckBox(b->label, parent);
			w->setChecked(b->defaultValue);
			return w;
		}
		if (auto* d = dynamic_cast<DoubleItem*>(item))
		{
			auto* w = new QDoubleSpinBox(parent);
			w->setRange(d->min, d->max);
			w->setSingleStep(d->step);
			w->setValue(d->defaultValue);
			return w;
		}
		if (auto* e = dynamic_cast<EnumItem*>(item))
		{
			auto* w = new QComboBox(parent);
			w->addItems(e->values);
			return w;
		}
		if (auto* btn = dynamic_cast<ButtonItem*>(item))
		{
			auto* w = new QPushButton(btn->label, parent);
			if (btn->onClick)
				QObject::connect(w, &QPushButton::clicked, btn->onClick);
			return w;
		}
		return nullptr;
	}

} // namespace SongYun
