#include "ui/MainWindow/MainWindow.h"
#include "ui/Ribbon/RibbonWidget.h"
#include "ui/project/ProjectPanel.h"
#include "view/View.h"
#include "app/App.h"
#include "selection/SelectionManager.h"
#include "view/ViewManager.h"
#include "common/EventBus.h"
#include "common/StatusService.h"
#include "command/CommandManager.h"

#include <QMenuBar>
#include <QToolBar>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QEvent>
#include <QStatusBar>
#include <any>

namespace SongYun {

	QMenuBar* MainWindow::menuBar() const      { return m_menubar; }
	QToolBar* MainWindow::toolBar() const      { return m_toolbar; }
	ActionManager& MainWindow::actionManager() { return *m_actionManager; }
	RibbonWidget* MainWindow::ribbon() const   { return m_ribbon; }

	MainWindow::MainWindow(Document* doc, QWidget* parent)
		: QMainWindow(parent)
	{
		m_actionManager = std::make_unique<ActionManager>(App::Instance().commandManager());

		initRibbon();
		initOCCTView(doc);

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

		m_projectPanel = new ProjectPanel(this);
		m_projectPanel->setDocument(doc);
		m_projectPanel->setView(m_view);

		auto* dock = new QDockWidget("Project", this);
		dock->setWidget(m_projectPanel);
		addDockWidget(Qt::LeftDockWidgetArea, dock);
	}

	void MainWindow::initRibbon() { m_ribbon = new RibbonWidget(this); }

	void MainWindow::initOCCTView(Document* doc)
	{
		auto* central = new QWidget(this);
		m_viewLayout = new QVBoxLayout(central);
		m_viewLayout->setContentsMargins(0, 0, 0, 0);
		m_viewLayout->setSpacing(0);

		m_viewLayout->addWidget(m_ribbon);

		m_toolbar = new QToolBar("Parameters", central);
		m_toolbar->setMovable(false);
		m_toolbar->setIconSize(QSize(16, 16));
		m_toolbar->setMinimumHeight(32);
		m_viewLayout->addWidget(m_toolbar);

		m_view = App::Instance().commandContext().viewManager().createView(doc, central);
		m_view->installEventFilter(this);
		m_viewLayout->addWidget(m_view, 1);

		setCentralWidget(central);
		App::Instance().commandManager().setParameterToolBar(m_toolbar);
	}

	void MainWindow::newDocument(Document* doc)
	{
		if (!doc || !m_viewLayout) return;

		// 1. 移除旧 View
		m_viewLayout->removeWidget(m_view);
		App::Instance().commandContext().viewManager().closeView(m_view);

		// 2. 创建新 View（绑定新 Document）
		m_view = App::Instance().commandContext().viewManager().createView(doc, centralWidget());
		m_view->installEventFilter(this);
		m_viewLayout->addWidget(m_view, 1);

		App::Instance().commandContext().selectionManager().SetView(m_view);

		// 3. 更新 ProjectPanel
		m_projectPanel->setDocument(doc);
		m_projectPanel->setView(m_view);
	}

	void MainWindow::initMenuBar()
	{
		m_menubar = new QMenuBar(this);
		setMenuBar(m_menubar);
	}

	void MainWindow::initStatusBar() { statusBar()->showMessage("Ready"); }
	bool MainWindow::eventFilter(QObject* obj, QEvent* event) { return QMainWindow::eventFilter(obj, event); }
	void MainWindow::initConnections() {}
	void MainWindow::initToolBar() {}

} // namespace SongYun
