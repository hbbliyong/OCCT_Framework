#include "setup/MenuBarSetup.h"
#include "command/ActionManager.h"

#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <qcheckbox.h>
#include <qcombobox.h>
#include "ui/VtkDialog/VtkDialog.h"

namespace Samples
{
	void MenuBarSetup::initialize(QMenuBar* menuBar, QToolBar* toolBar,
		SongYun::ActionManager& am)
	{
		// ========== 菜单栏 ==========
		QMenu* fileMenu = menuBar->addMenu("File");

		QAction* openAction = fileMenu->addAction("Open");
		QObject::connect(openAction, &QAction::triggered, [menuBar]()
			{
				auto dialog = new SongYun::VtkDialog(menuBar);
				dialog->show();
			});

		fileMenu->addSeparator();
		fileMenu->addAction(am.createCommandAction({ "Samples.CreateCylinder" }));
		fileMenu->addAction(am.createCommandAction({ "Samples.CreatePolyline" }));

		fileMenu->addSeparator();
		fileMenu->addAction(am.createCommandAction({ "Samples.PickBuild" }));
		fileMenu->addAction(am.createCommandAction({ "Samples.DrawPolylineRubber" }));

		// ========== 工具栏 ==========
		toolBar->addSeparator();
		toolBar->addAction("Custom Surface");
		toolBar->addWidget(new QCheckBox("Custom Surface"));
		QComboBox* layerCombo = new QComboBox();
		layerCombo->addItems({ "图层0", "墙体", "标注", "电气" });
		toolBar->addWidget(layerCombo);
		toolBar->addAction(am.createCommandAction({ "Samples.CreateCylinder",        "Cylinder" }));
		toolBar->addAction(am.createCommandAction({ "Samples.CreatePolyline",        "Polyline" }));
		toolBar->addAction(am.createCommandAction({ "Samples.PickBuild",             "Build" }));
		toolBar->addAction(am.createCommandAction({ "Samples.DrawPolylineRubber",    "RubberPoly" }));
	}

} // namespace Samples
