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
#include "document/ProjectManager.h"
#include "document/Project.h"
#include "document/Document.h"
#include "app/App.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>

namespace Samples
{
	static void registerViewCommands()
	{
		auto& reg = SongYun::CommandRegistry::Instance();
		reg.registerDescriptor("Samples.View3D",  [] { return new SetViewCommand(SetViewCommand::Axo); });
		reg.registerDescriptor("Samples.ViewXOY", [] { return new SetViewCommand(SetViewCommand::Top); });
		reg.registerDescriptor("Samples.ViewXOZ", [] { return new SetViewCommand(SetViewCommand::Front); });
		reg.registerDescriptor("Samples.ViewYOZ", [] { return new SetViewCommand(SetViewCommand::Left); });
	}

	// 当前文档路径（简单静态存储）
	static QString s_currentFilePath;

	void MenuBarSetup::initialize(SongYun::MainWindow& mainWindow)
	{
		registerViewCommands();

		auto& am = mainWindow.actionManager();
		auto* ribbon = mainWindow.ribbon();

		// ========== Ribbon（不变）==========
		SongYun::RibbonModel model;

		model.addPage({
			"Home",
			{
				{ "Primitive",   { { "Cylinder", "Samples.CreateCylinder" } } },
				{ "Modify",      {
					{ "Build (Fuse)",      "Samples.PickBuild" },
					{ "Rubber Poly",       "Samples.DrawPolylineRubber" },
					{ "Triangulate Poly",  "Samples.TriangulatePolyline" },
				}},
			}
		});
		model.addPage({
			"Draw",
			{
				{ "Curve",       { {"Line","Samples.DrawLine"}, {"Arc","Samples.DrawArc"}, {"Circle","Samples.DrawCircle"} } },
				{ "Polyline",    { {"Polyline","Samples.CreatePolyline"}, {"Rubber Polyline","Samples.DrawPolylineRubber"} } },
			}
		});
		model.addPage({
			"View",
			{
				{ "Orientation", {
					{"3D","Samples.View3D"}, {"XOY","Samples.ViewXOY"}, {"XOZ","Samples.ViewXOZ"}, {"YOZ","Samples.ViewYOZ"}
				}},
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
		auto* menu = mainWindow.menuBar();
		QMenu* fileMenu = menu->addMenu("File");

		// New — 新 Project → 新 Document → 新 View
		QAction* newAction = fileMenu->addAction("New");
		QObject::connect(newAction, &QAction::triggered, [&mainWindow]() {
			auto* proj = SongYun::ProjectManager::Instance().createProject("NewProject");
			mainWindow.newDocument(proj->document());
			s_currentFilePath.clear();
		});

		// Open
		QAction* openAction = fileMenu->addAction("Open");
		QObject::connect(openAction, &QAction::triggered, [&mainWindow]() {
			QString path = QFileDialog::getOpenFileName(nullptr, "Open", "",
				"OCCT Files (*.brep *.step *.stp *.iges *.igs);;All Files (*)");
			if (path.isEmpty()) return;

			auto* proj = SongYun::ProjectManager::Instance().createProject("Opened");
			if (proj->document()->load(path.toStdString()))
			{
				proj->document()->loadPropertiesFromTDF();
				mainWindow.newDocument(proj->document());
				s_currentFilePath = path;
			}
			else
			{
				QMessageBox::warning(nullptr, "Error", "Failed to open file.");
			}
		});

		fileMenu->addSeparator();

		// Save
		QAction* saveAction = fileMenu->addAction("Save");
		QObject::connect(saveAction, &QAction::triggered, []() {
			auto* proj = SongYun::ProjectManager::Instance().activeProject();
			if (!proj) return;

			auto* doc = proj->document();
			if (s_currentFilePath.isEmpty())
			{
				QString path = QFileDialog::getSaveFileName(nullptr, "Save As", "",
					"BREP Files (*.brep);;All Files (*)");
				if (path.isEmpty()) return;
				s_currentFilePath = path;
			}

			doc->savePropertiesToTDF();
			if (!doc->save(s_currentFilePath.toStdString()))
				QMessageBox::warning(nullptr, "Error", "Failed to save file.");
		});

		// Save As
		QAction* saveAsAction = fileMenu->addAction("Save As...");
		QObject::connect(saveAsAction, &QAction::triggered, []() {
			auto* proj = SongYun::ProjectManager::Instance().activeProject();
			if (!proj) return;

			QString path = QFileDialog::getSaveFileName(nullptr, "Save As", "",
				"BREP Files (*.brep);;All Files (*)");
			if (path.isEmpty()) return;

			auto* doc = proj->document();
			doc->savePropertiesToTDF();
			if (doc->save(path.toStdString()))
				s_currentFilePath = path;
			else
				QMessageBox::warning(nullptr, "Error", "Failed to save file.");
		});

		fileMenu->addSeparator();

		// Exit
		QAction* exitAction = fileMenu->addAction("Exit");
		QObject::connect(exitAction, &QAction::triggered, []() {
			QApplication::quit();
		});
	}

} // namespace Samples
