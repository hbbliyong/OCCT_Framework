#include "setup/MenuBarSetup.h"
#include "ui/MainWindow/MainWindow.h"
#include "ui/Ribbon/RibbonWidget.h"
#include "ui/Ribbon/RibbonPageWidget.h"
#include "ui/ribbon/RibbonBuilder.h"
#include "ui/ribbon/RibbonModel.h"
#include "command/ActionManager.h"
#include "command/CommandManager.h"
#include "command/CommandRegistry.h"
#include "cmds/SetViewCommand.h"
#include "app/App.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QPushButton>

namespace Samples
{
	static void registerViewCommands()
	{
		SongYun::CommandRegistry::Instance().registerDescriptor(
			"Samples.View3D", [] { return new SetViewCommand(SetViewCommand::Axo); });
		SongYun::CommandRegistry::Instance().registerDescriptor(
			"Samples.ViewXOY", [] { return new SetViewCommand(SetViewCommand::Top); });
		SongYun::CommandRegistry::Instance().registerDescriptor(
			"Samples.ViewXOZ", [] { return new SetViewCommand(SetViewCommand::Front); });
		SongYun::CommandRegistry::Instance().registerDescriptor(
			"Samples.ViewYOZ", [] { return new SetViewCommand(SetViewCommand::Left); });
	}

	void MenuBarSetup::initialize(SongYun::MainWindow& mainWindow)
	{
		registerViewCommands();

		auto& am = mainWindow.actionManager();
		auto* ribbon = mainWindow.ribbon();

		// ========== Ribbon ==========
		SongYun::RibbonModel model;

		model.addPage({
			"Home",
			{
				{ "Primitive",
					{ { "Cylinder", "Samples.CreateCylinder" } }
				},
				{ "Modify",
					{
						{ "Build (Fuse)",      "Samples.PickBuild" },
						{ "Rubber Poly",       "Samples.DrawPolylineRubber" },
						{ "Triangulate Poly",  "Samples.TriangulatePolyline" },
					}
				},
			}
		});

		model.addPage({
			"Draw",
			{
				{ "Curve",
					{
						{ "Line",       "Samples.DrawLine" },
						{ "Arc",        "Samples.DrawArc" },
						{ "Circle",     "Samples.DrawCircle" },
					}
				},
				{ "Polyline",
					{
						{ "Polyline",        "Samples.CreatePolyline" },
						{ "Rubber Polyline", "Samples.DrawPolylineRubber" },
					}
				},
			}
		});

		model.addPage({
			"View",
			{
				{ "Orientation",
					{
						{ "3D",   "Samples.View3D" },
						{ "XOY",  "Samples.ViewXOY" },
						{ "XOZ",  "Samples.ViewXOZ" },
						{ "YOZ",  "Samples.ViewYOZ" },
					}
				},
			}
		});

		SongYun::RibbonBuilder builder;
		builder.build(ribbon, model, am);

		// Undo / Redo
		auto& cmdMgr = SongYun::App::Instance().commandManager();
		auto* quickPage = ribbon->addPage("Quick");
		auto* editGroup = quickPage->getOrCreateGroup("Edit");
		auto* undoBtn = new QPushButton("Undo");
		auto* redoBtn = new QPushButton("Redo");
		QObject::connect(undoBtn, &QPushButton::clicked, [&cmdMgr]() { cmdMgr.undo(); });
		QObject::connect(redoBtn, &QPushButton::clicked, [&cmdMgr]() { cmdMgr.redo(); });
		editGroup->contentLayout()->addWidget(undoBtn);
		editGroup->contentLayout()->addWidget(redoBtn);

		// ========== 菜单栏 ==========
		QMenu* fileMenu = mainWindow.menuBar()->addMenu("File");
		fileMenu->addAction(am.createCommandAction({ "Samples.CreateCylinder", "Cylinder" }));
		fileMenu->addAction(am.createCommandAction({ "Samples.PickBuild", "Build (Fuse)" }));
		fileMenu->addSeparator();
		fileMenu->addAction(am.createCommandAction({ "Samples.DrawLine", "Line" }));
		fileMenu->addAction(am.createCommandAction({ "Samples.DrawArc", "Arc" }));
		fileMenu->addAction(am.createCommandAction({ "Samples.DrawCircle", "Circle" }));
		fileMenu->addSeparator();
		fileMenu->addAction(am.createCommandAction({ "Samples.CreatePolyline", "Polyline" }));
		fileMenu->addAction(am.createCommandAction({ "Samples.DrawPolylineRubber", "Rubber Polyline" }));
		fileMenu->addSeparator();
		fileMenu->addAction(am.createCommandAction({ "Samples.View3D", "3D View" }));
		fileMenu->addAction(am.createCommandAction({ "Samples.ViewXOY", "XOY View" }));
		fileMenu->addAction(am.createCommandAction({ "Samples.ViewXOZ", "XOZ View" }));
		fileMenu->addAction(am.createCommandAction({ "Samples.ViewYOZ", "YOZ View" }));
	}

} // namespace Samples
