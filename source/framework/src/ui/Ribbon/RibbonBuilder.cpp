#include "ui/ribbon/RibbonBuilder.h"
#include "ui/Ribbon/RibbonWidget.h"
#include "ui/Ribbon/RibbonPageWidget.h"
#include "ui/Ribbon/RibbonGroupWidget.h"
#include "command/ActionManager.h"
#include <QAction>
#include <QPushButton>
#include <QHBoxLayout>

namespace SongYun {

	void RibbonBuilder::build(RibbonWidget* ribbon, const RibbonModel& model,
							  ActionManager& am)
	{
		if (!ribbon) return;

		for (auto& pageDef : model.pages())
		{
			auto* page = ribbon->addPage(pageDef.name);

			for (auto& groupDef : pageDef.groups)
			{
				auto* group = page->getOrCreateGroup(groupDef.name);

				for (auto& item : groupDef.items)
				{
					if (!item.commandId) continue;
					auto* action = am.createCommandAction({ item.commandId, item.text });

					auto* btn = new QPushButton(item.text);
					btn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
					btn->setMinimumWidth(btn->sizeHint().width() + 16);
					QObject::connect(btn, &QPushButton::clicked, action, &QAction::trigger);
					group->contentLayout()->addWidget(btn);
				}

				group->contentLayout()->addStretch();
			}

			page->layout()->addStretch();
		}
	}

} // namespace SongYun
