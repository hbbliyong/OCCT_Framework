#include "ui/MainWindow/MainWindow.h"
#include "view/View.h"
#include "app/App.h"
#include "selection/SelectionManager.h"
#include "view/ViewManager.h"
#include "document/DocumentManager.h"
#include "common/EventBus.h"
#include "common/StatusService.h"
#include "command/ActionManager.h"

#include <QMenuBar>
#include <QToolBar>
#include <QEvent>
#include <QStatusBar>
#include <any>

namespace SongYun {

	MainWindow& MainWindow::Instance()
	{
		static MainWindow instance;
		return instance;
	}

	QMenuBar* MainWindow::menuBar() const { return myMenuBar; }
	QToolBar* MainWindow::toolBar() const { return myToolBar; }
	ActionManager& MainWindow::actionManager() { return *m_actionManager; }

	MainWindow::MainWindow(QWidget* parent)
		: QMainWindow(parent)
	{
		m_actionManager  = std::make_unique<ActionManager>(App::Instance().commandManager());

		initOCCTView();

		App::Instance().eventBus().subscribe("status.message",
			[this](const std::any& data)
			{
				auto* ev = std::any_cast<StatusMessageEvent>(&data);
				if (ev) statusBar()->showMessage(ev->text, ev->timeout ? ev->timeout : 5000);
			});

		initMenuBar();
		initStatusBar();
		initConnections();
		initToolBar();
	}

	void MainWindow::close() {}

	void MainWindow::initOCCTView()
	{
		auto doc = DocumentManager::Instance().createDocument("default");
		view_occt = App::Instance().commandContext().viewManager().createView(doc.get(), this);
		view_occt->installEventFilter(this);
		setCentralWidget(view_occt);
	}

	void MainWindow::initMenuBar()
	{
		myMenuBar = new QMenuBar(this);
		setMenuBar(myMenuBar);
	}

	void MainWindow::initStatusBar() { statusBar()->showMessage("Ready"); }

	bool MainWindow::eventFilter(QObject* obj, QEvent* event)
	{
		return QMainWindow::eventFilter(obj, event);
	}

	void MainWindow::initConnections() {}

	void MainWindow::initToolBar()
	{
		myToolBar = addToolBar("Main Toolbar");
	}

} // namespace SongYun
