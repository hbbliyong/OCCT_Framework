#include "ui/MainWindow/MainWindow.h"
#include "ui/VtkDialog/VtkDialog.h"
#include "view/OccView.h"
#include "command/CommandRegistry.h"
#include "command/CommandFactory.h"
#include "command/InteractiveCommand.h"
#include "selection/PickService.h"

#include <QMenuBar.h>
#include <QMessageBox.h>
#include <QToolBar.h>

#include <QEvent>
#include <QStatusBar.h>
#include <AIS_Shape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <gp_Ax2.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <Geom_Circle.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <Geom_ConicalSurface.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
// 前置声明（避免把第三方头直接放在所有头中）
class IVtkOCC_Shape;

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	CommandRegistry::registerAllCommands();

	initOCCTView();
	// auto view = new OccView(this);
	// connect(view, &OccView::initialized, [view]() {
	//	view->displayBox(); // 确保此时已初始化
	//	});
	// setCentralWidget(view);
	// view->show();

	initMenuBar();
	initStatusBar();
	initConnections();
	initToolBar();
}

void MainWindow::close()
{
}

void MainWindow::initOCCTView()
{
	myViewer = Viewer(V3d_XposYnegZpos);
	myContext = new AIS_InteractiveContext(myViewer);
	myContext->SetDisplayMode(AIS_Shaded, Standard_True); // 着色模式
	myContext->SetAutomaticHilight(Standard_True);		  // 启用自动高亮
	view_occt = new View(myContext, true, this);
	view_occt->installEventFilter(this);
	setCentralWidget(view_occt);
	PickService::instance().initialize(view_occt, myContext);
}

void MainWindow::initMenuBar()
{
	QMenuBar *menuBar = new QMenuBar(this);
	setMenuBar(menuBar);
	QMenu *fileMenu = menuBar->addMenu("File");
	// fileMenu->addAction("Exit", this, SLOT(close()));
	QAction *openAction = fileMenu->addAction("Open");
	connect(openAction, &QAction::triggered, this, [this](bool checked = false)
			{
		QMessageBox::information(this, "Open", "Open action triggered!");
		auto dialog = new VtkDialog(this);
		dialog->show(); });

	fileMenu->addSeparator();
	QAction *cylinderAction = fileMenu->addAction("Create Cylinder");
	connect(cylinderAction, &QAction::triggered, this, [this](bool checked = false)
			{
			std::map<QString, std::any> params;
			params["context"] = myContext;
			params["statusCallback"] = std::function<void(const QString &)>([this](const QString &message) { updateStatus(message); });
			auto command = CommandFactory::instance().create("CreateCylinder", params);
			if (command)
			{
				setCurrentCommand(command);
			} });

	QAction *polylineAction = fileMenu->addAction("Create Polyline");
	connect(polylineAction, &QAction::triggered, this, [this](bool checked = false)
			{
		std::map<QString, std::any> params;
		params["context"] = myContext;
		params["statusCallback"] = std::function<void(const QString &)>([this](const QString &message) { updateStatus(message); });
		auto command = CommandFactory::instance().create("CreatePolyline", params);
		if (command)
		{
			setCurrentCommand(command);
		} });

	QAction *exitAction = fileMenu->addAction("Create");
	connect(exitAction, &QAction::triggered, this, [this](bool checked = false)
			{ runModelData(); });
}

void MainWindow::initStatusBar()
{
	statusBar()->showMessage("Ready");
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
	if (obj == view_occt && event->type() == QEvent::MouseButtonRelease)
	{
		QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
		if (myCurrentCommand)
		{
			onViewMouseRelease(mouseEvent);
			return true;
		}
	}

	return QMainWindow::eventFilter(obj, event);
}

void MainWindow::initConnections()
{
	connect(view_occt, &View::selectionChanged, this, &MainWindow::processSelectChanged);
}

void MainWindow::initToolBar()
{
	QToolBar *toolBar = addToolBar("Main Toolbar");
	auto customFaceAction = toolBar->addAction("Custom Surface");
	connect(customFaceAction, &QAction::triggered, this, [this](bool checked = false)
			{ displayCustormSurface(); });
}

void MainWindow::runModelData()
{
	// 1. 创建几何形状
	TopoDS_Shape box = BRepPrimAPI_MakeBox(100, 200, 300).Shape();

	// 2. 包装为AIS交互对象
	Handle(AIS_Shape) aisBox = new AIS_Shape(box);

	// 3. 通过AIS_InteractiveContext显示（关键步骤）
	myContext->Display(aisBox, Standard_True); // Standard_True表示立即刷新
}

void MainWindow::processSelectChanged()
{
	myContext->InitSelected();

	TopoDS_Shape selShape = myContext->SelectedShape();
	if (!selShape.IsNull() && selShape.ShapeType() == TopAbs_EDGE)
	{
		// TopoDS_Edge edge = TopoDS::Edge(selShape);

		// Handle(AIS_InteractiveObject) selected = myContext->SelectedInteractive();
		// Handle(MAisDemo) sel = Handle(MAisDemo)::DownCast(selected);
		// TopoDS_Shape aShape = sel->Shape();
		//
		// BRepFilletAPI_MakeFillet  MFFillet(aShape, ChFi3d_Polynomial);
		// TopExp_Explorer  ex(aShape , TopAbs_EDGE);
		// double r = 2.;
		// MFFillet.Add(r , edge);
		// Handle(AIS_InteractiveObject) aisFillet = new MAisDemo(MFFillet.Shape());

		// myContext->Remove(selected , false);
		// myContext->Display(aisFillet , 1 , 0 , true);
		// myContext->Activate(aisFillet , MAisDemo::SelectionMode(TopAbs_EDGE));
	}
}

void MainWindow::displayCustormSurface()
{
	gp_Ax2 axis(gp_Pnt(0, 200, 0), gp_Dir(0, 0, 1)); // Z轴为圆柱方向
	auto halfCylinder = BRepPrimAPI_MakeCylinder(axis, 30, 100, M_PI);
	TopoDS_Face halfCylinderFace = halfCylinder.Face();

	gp_Ax2 circleAxis(gp_Pnt(0, 0, 0), gp_Dir(0, 1, 0));
	gp_Circ circle(circleAxis, 30);
	GC_MakeArcOfCircle arcMaker(circle, 0.0, M_PI, Standard_True);
	TopoDS_Edge arcEdge = BRepBuilderAPI_MakeEdge(arcMaker.Value()).Edge();

	myContext->Display(new AIS_Shape(halfCylinderFace), Standard_True);
	myContext->Display(new AIS_Shape(arcEdge), Standard_True);

	// 圆锥面
	gp_Ax3 axis3(gp_Pnt(150, 150, 150), gp_Dir(0, 0, 1)); // 圆柱轴线
	Handle(Geom_ConicalSurface) coneSurface = new Geom_ConicalSurface(axis3, M_PI / 6.0, 100);
	TopoDS_Face coneFace = BRepBuilderAPI_MakeFace(coneSurface, 0.0, M_PI / 2.0, 0.0, 100.0, Precision::Confusion()).Face();
	myContext->Display(new AIS_Shape(coneFace), Standard_True);
}

Handle(V3d_Viewer) MainWindow::Viewer(const V3d_TypeOfOrientation theViewProj)
{
	static Handle(OpenGl_GraphicDriver) aGraphicDriver;
	Handle(Aspect_DisplayConnection) aDisplayConnection;
	aGraphicDriver = new OpenGl_GraphicDriver(aDisplayConnection);
	Handle(V3d_Viewer) aViewer = new V3d_Viewer(aGraphicDriver);
	aViewer->SetDefaultViewProj(theViewProj);

	aViewer->SetDefaultLights();
	aViewer->SetLightOn();
	return aViewer;
}

void MainWindow::updateStatusBar(const std::vector<int> &aNewPos)
{
}

void MainWindow::updateStatus(const QString &message)
{
	statusBar()->showMessage(message, 5000);
}

void MainWindow::setCurrentCommand(const std::shared_ptr<Command> &command)
{
	myCurrentCommand = command;
	if (myCurrentCommand)
	{
		myCommandManager.execute(myCurrentCommand);
	}
}

void MainWindow::onViewMouseRelease(QMouseEvent *event)
{
	if (!view_occt || !myCurrentCommand)
		return;

	auto interactiveCmd = std::dynamic_pointer_cast<InteractiveCommand>(myCurrentCommand);
	if (!interactiveCmd)
		return;

	if (event->button() == Qt::LeftButton)
	{
		if (interactiveCmd->requiresPointPick())
		{
			const gp_Pnt worldPoint = PickService::instance().pickPoint(event->x(), event->y());
			interactiveCmd->onPickedPoint(worldPoint);
		}
		else if (interactiveCmd->requiresObjectPick())
		{
			const PickResult result = PickService::instance().pickObject(event->x(), event->y());
			interactiveCmd->onPickedObject(result);
		}
	}
	else if (event->button() == Qt::RightButton)
	{
		interactiveCmd->onFinish();
		myCurrentCommand.reset();
	}
}
