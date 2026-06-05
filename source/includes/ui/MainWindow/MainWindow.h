#pragma once

#include <memory>
#include <functional>
#include <QEvent>
#include <QMainWindow>
#include <AIS_InteractiveContext.hxx>
#include <V3d_Viewer.hxx>
#include "command/CommandManager.h"
#include "command/Command.h"
#include "view/View.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow() = default;
	void close();

private:
	void initOCCTView();
	void initMenuBar();
	void initStatusBar();
	void initConnections();
	void initToolBar();

	void runModelData();
	void processSelectChanged();
	void displayCustormSurface();
	void setCurrentCommand(const std::shared_ptr<Command> &command);
	void updateStatus(const QString &message);

	bool eventFilter(QObject *obj, QEvent *event) override;
	void onViewMouseRelease(QMouseEvent *event);

private:
	Handle(V3d_Viewer) Viewer(const V3d_TypeOfOrientation theViewProj);

	void updateStatusBar(const std::vector<int> &aNewPos);

	Handle(V3d_Viewer) myViewer;
	Handle(AIS_InteractiveContext) myContext;
	std::vector<int> myClickPos;
	std::vector<Handle(AIS_InteractiveObject)> objs;

	View *view_occt;
	CommandManager myCommandManager;
	std::shared_ptr<Command> myCurrentCommand;
};