#include "view/OccView.h"
#include <BRepPrimAPI_MakeBox.hxx>
#include <AIS_Shape.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <WNT_Window.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <gp_Ax2.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Geom_Circle.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>

#include <V3d_Viewer.hxx>
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QMouseEvent>

OccView::OccView(QWidget *parent) : QOpenGLWidget(parent)
{
	setFocusPolicy(Qt::StrongFocus);
	setMouseTracking(true);
}

void OccView::initializeGL()
{
	// 1. 配置OpenGL上下文（必须≥3.3 Core Profile）
	// 1. 检查上下文是否激活
	if (!context()->isValid())
	{
		qFatal("OpenGL context creation FAILED!"); // 立即终止，避免后续崩溃
	}

	// 2. 打印实际支持的 OpenGL 版本
	qDebug() << "GL Version:" << glGetString(GL_VERSION);
	qDebug() << "GL Renderer:" << glGetString(GL_RENDERER);
	// 2. 创建OCCT图形驱动
	Handle(OpenGl_GraphicDriver) driver = new OpenGl_GraphicDriver(
		new Aspect_DisplayConnection());

	// 3. 初始化Viewer和View
	m_viewer = new V3d_Viewer(driver);
	m_viewer->SetDefaultLights();
	m_viewer->SetLightOn();
	m_view = m_viewer->CreateView();
	auto aWindow = new WNT_Window((Aspect_Handle)this->winId());
	m_view->SetWindow(aWindow);
	m_view->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);

	// 4. 创建交互式上下文（AIS核心）
	m_context = new AIS_InteractiveContext(m_viewer);
	m_context->SetDisplayMode(AIS_Shaded, Standard_True); // 着色模式
	m_context->SetAutomaticHilight(Standard_True);		  // 启用自动高亮
}

void OccView::resizeGL(int w, int h)
{
	m_view->MustBeResized();
}

void OccView::paintGL()
{
	m_view->Redraw(); // 触发OCCT渲染
}
void OccView::displayBox()
{
	//// 1. 创建几何形状
	// TopoDS_Shape box = BRepPrimAPI_MakeBox(100, 100, 100).Shape();

	//// 2. 包装为AIS交互对象
	// Handle(AIS_Shape) aisBox = new AIS_Shape(box);

	//// 3. 通过AIS_InteractiveContext显示（关键步骤）
	// m_context->Display(aisBox, Standard_True); // Standard_True表示立即刷新
	// m_view->FitAll(); // 自动缩放到窗口
	this->displaySurface();
}

void OccView::displaySurface()
{
	// 圆柱参数
	double radius = 10.0;
	double height = 50.0;

	// 1. 构建圆柱面
	gp_Ax2 axis(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1)); // Z轴为圆柱方向
	Handle(Geom_CylindricalSurface) cyl = new Geom_CylindricalSurface(axis, radius);

	// 2. 构建半圆弧作为竖向边
	// 假设半圆弧在 XZ 平面，从 (radius,0,0) 到 (-radius,0,0)
	gp_Pnt p1(radius, 0, 0);
	gp_Pnt p2(-radius, 0, 0);
	gp_Pnt center(0, 0, height / 2); // 半圆弧圆心在圆柱中部
	Handle(Geom_TrimmedCurve) arc = GC_MakeArcOfCircle(p1, center, p2);

	// 3. 构建边
	TopoDS_Edge edgeArc = BRepBuilderAPI_MakeEdge(arc);

	// 4. 构建面
	// 如果原本有水平边可以构建一个封闭的 Wire
	TopoDS_Edge topEdge = BRepBuilderAPI_MakeEdge(gp_Pnt(radius, 0, height), gp_Pnt(-radius, 0, height));
	TopoDS_Edge bottomEdge = BRepBuilderAPI_MakeEdge(gp_Pnt(radius, 0, 0), gp_Pnt(-radius, 0, 0));
	BRepBuilderAPI_MakeWire wireMaker;
	wireMaker.Add(edgeArc);
	wireMaker.Add(topEdge);
	wireMaker.Add(bottomEdge);
	TopoDS_Wire wire = wireMaker.Wire();

	TopoDS_Face face = BRepBuilderAPI_MakeFace(cyl, wire);

	// 2. 包装为AIS交互对象
	Handle(AIS_Shape) aisFace = new AIS_Shape(face);
	m_context->Display(aisFace, Standard_True); // Standard_True表示立即刷新
	m_view->FitAll();							// 自动缩放到窗口
}

void OccView::mousePressEvent(QMouseEvent *event)
{
	if (event->button() != Qt::LeftButton)
		return;

	// 1. 坐标转换：Qt坐标 → OCCT视图坐标（Y轴必须翻转！）
	const int x = event->x();
	const int y = height() - event->y(); // 关键：OCCT Y轴方向与Qt相反

	// 2. 【必需】检测鼠标位置的对象（触发高亮预览）
	m_context->MoveTo(x, y, m_view, Standard_True);

	// 3. 【关键】通过SelectDetected()确认选择（必须指定选择方案）
	if (m_context->MoreDetected())
	{
		m_context->SelectDetected(AIS_SelectionScheme_Replace); // 替换当前选择
		update();												// 触发重绘以更新高亮状态
	}
}

void OccView::mouseMoveEvent(QMouseEvent *event)
{
}

void OccView::wheelEvent(QWheelEvent *event)
{
}
void OccView::showEvent(QShowEvent *event)
{
	QOpenGLWidget::showEvent(event);

	if (!m_isInitialized && winId() != 0)
	{
		// ...[您的OCCT初始化代码]...

		m_isInitialized = true;

		// 关键：初始化完成后立即发出信号！
		emit initialized(); // ← 这行不能漏！
	}
}