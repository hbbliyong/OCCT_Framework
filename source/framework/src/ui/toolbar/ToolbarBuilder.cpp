#include "ui/toolbar/ToolbarBuilder.h"
#include <QHBoxLayout>
#include <QLabel>

namespace SongYun {

	QWidget* ToolbarBuilder::build(const CommandUI& ui, QWidget* parent)
	{
		auto* panel = new QWidget(parent);
		auto* layout = new QHBoxLayout(panel);
		layout->setContentsMargins(6, 2, 6, 2);
		layout->setSpacing(8);

		bool firstSection = true;
		for (auto& section : ui.sections())
		{
			if (!firstSection)
			{
				auto* sep = new QLabel("|", panel);
				sep->setStyleSheet("color: gray;");
				layout->addWidget(sep);
			}
			firstSection = false;

			for (auto& item : section.items())
			{
				auto* base = item.get();
				if (dynamic_cast<BoolItem*>(base) || dynamic_cast<ButtonItem*>(base))
				{
					// CheckBox/Button 自带 label
				}
				else if (!base->label.isEmpty())
				{
					layout->addWidget(new QLabel(base->label + ":", panel));
				}

				auto* w = m_factory.create(base, panel);
				if (w)
				{
					layout->addWidget(w);
					m_binding.bind(w, base);
				}
			}
		}

		layout->addStretch();
		return panel;
	}

	void ToolbarBuilder::clear() { m_binding.unbindAll(); }

} // namespace SongYun
