#include "ui/MainWindow/MainWindow.h"
#include "ui/VtkDialog/VtkDialog.h"
#include "view/View.h"
#include "app/App.h"
#include "selection/SelectionManager.h"

#include <view/ViewManager.h>
#include "command/InteractiveCommand.h"
#include "selection/PickService.h"

#include <QMenuBar>
#include <QMessageBox>
#include <QToolBar>

#include <QEvent>
#include <QStatusBar>

#include <gp_Pnt.hxx>

class IVtkOCC_Shape;

namespace SongYun {

	// 单例实现
	MainWindow& MainWindow::Instance()
	{
		static MainWindow instance;
		return instance;
	}

	// 获取菜单栏
	QMenuBar* MainWindow::menuBar() const
	{
		return myMenuBar;
	}

	// 获取工具栏
	QToolBar* MainWindow::toolBar() const
	{
		return myToolBar;
	}

	MainWindow::MainWindow(QWidget* parent)
		: QMainWindow(parent), myMenuBar(nullptr), myToolBar(nullptr)
	{
		initOCCTView();


		initMenuBar();
		initStatusBar();
		initConnections();
		initToolBar();
	}

	void MainWindow::close()
	{}

	void MainWindow::initOCCTView()
	{
		view_occt = new View(this);
		view_occt->installEventFilter(this);
		App::Instance().commandContext().selectionManager().SetView(view_occt);
		App::Instance().commandContext().viewManager().setActiveView(view_occt);
		connect(view_occt, &View::viewReady, []() {
			//需要依赖View的初始化完成后再进行一些操作
	   //view_occt	->displayBox(); // 确保此时已初始化
			});
		setCentralWidget(view_occt);
	}

	void MainWindow::initMenuBar()
	{
		myMenuBar = new QMenuBar(this);
		setMenuBar(myMenuBar);
		QMenu* fileMenu = myMenuBar->addMenu("File");
		// fileMenu->addAction("Exit", this, SLOT(close()));
		QAction* openAction = fileMenu->addAction("Open");
		connect(openAction, &QAction::triggered, this, [this](bool checked = false)
			{
				QMessageBox::information(this, "Open", "Open action triggered!");
				auto dialog = new VtkDialog(this);
				dialog->show(); });

		fileMenu->addSeparator();
		QAction* cylinderAction = fileMenu->addAction("Create Cylinder");
		connect(cylinderAction, &QAction::triggered, this, [this](bool checked = false)
			{
				CommandManager::Instance().execute("Samples.CreateCylinder");
			}
		);

		QAction* polylineAction = fileMenu->addAction("Create Polyline");
		connect(polylineAction, &QAction::triggered, this, [this](bool checked = false)
			{
				CommandManager::Instance().execute("Samples.CreatePolyline");
			}
		);

		QAction* exitAction = fileMenu->addAction("Create");
		connect(exitAction, &QAction::triggered, this, [this](bool checked = false)
			{});
	}

	void MainWindow::initStatusBar()
	{
		statusBar()->showMessage("Ready");
	}

	bool MainWindow::eventFilter(QObject* obj, QEvent* event)
	{
		//if (obj == view_occt && event->type() == QEvent::MouseButtonRelease)
		//{
		//	QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
		//	if (myCurrentCommand)
		//	{
		//		onViewMouseRelease(mouseEvent);
		//		return true;
		//	}
		//}

		return QMainWindow::eventFilter(obj, event);
	}

	void MainWindow::initConnections()
	{
		//connect(view_occt, &OccView::selectionChanged, this, &MainWindow::processSelectChanged);
	}

	void MainWindow::initToolBar()
	{
		myToolBar = addToolBar("Main Toolbar");
		auto customFaceAction = myToolBar->addAction("Custom Surface");
		connect(customFaceAction, &QAction::triggered, this, [this](bool checked = false)
			{});
	}

	void MainWindow::updateStatusBar(const std::vector<int>& aNewPos)
	{}

	void MainWindow::updateStatus(const QString& message)
	{
		statusBar()->showMessage(message, 5000);
	}


	void MainWindow::onViewMouseRelease(QMouseEvent* event)
	{

	}

} // namespace SongYun
