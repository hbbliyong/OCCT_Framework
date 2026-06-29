#include "view/View.h"
#include "document/Document.h"
#include "common/EventBus.h"
#include "app/App.h"

static QCursor* defCursor = nullptr;
static QCursor* handCursor = nullptr;
static QCursor* panCursor = nullptr;
static QCursor* globPanCursor = nullptr;
static QCursor* zoomCursor = nullptr;
static QCursor* rotCursor = nullptr;

#include "tool/ToolManager.h"
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
#include <Aspect_NeutralWindow.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>

#include <V3d_Viewer.hxx>
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QMouseEvent>

namespace SongYun {
	View::View(Document* doc, QWidget* parent)
		: QOpenGLWidget(parent),
		myIs3dView(true),
		dpr(devicePixelRatioF()),
		myIsRaytracing(false),
		myIsShadowsEnabled(true),
		myIsReflectionsEnabled(false),
		myIsAntialiasingEnabled(false),
		m_doc(doc)
	{
		myCurrentMode = CurrentAction3d_Nothing;
		setFocusPolicy(Qt::StrongFocus);
		setMouseTracking(true);

		setBackgroundRole(QPalette::NoRole);
		setAttribute(Qt::WA_DontCreateNativeAncestors);
		setUpdateBehavior(QOpenGLWidget::NoPartialUpdate);
		setAttribute(Qt::WA_OpaquePaintEvent);

		// 构造即绑定：注册为 Document 观察者
		if (m_doc)
			m_doc->addObserver(this);
	}

	void View::initializeGL()
	{
		// 1. 配置OpenGL上下文（必须≥3.3 Core Profile）
		// 1. 检查上下文是否激活
		if (!context()->isValid())
		{
			qFatal("OpenGL context creation FAILED!"); // 立即终止，避免后续崩溃
		}

		// 2. 打印实际支持的 OpenGL 版本
		qDebug() << "GL Version:" << reinterpret_cast<const char*>(glGetString(GL_VERSION));
		qDebug() << "GL Renderer:" << reinterpret_cast<const char*>(glGetString(GL_RENDERER));
		qDebug() << "GL Extensions:" << reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
		qDebug() << QOpenGLContext::currentContext()->format();
		qDebug() << "ctx:" << QOpenGLContext::currentContext();
		qDebug() << "valid:" << QOpenGLContext::currentContext()->isValid();
		// 2. 创建OCCT图形驱动
		Handle(OpenGl_GraphicDriver) driver = new OpenGl_GraphicDriver(
			new Aspect_DisplayConnection());

		// 3. 初始化Viewer和View
		m_viewer = new V3d_Viewer(driver);
		m_viewer->SetDefaultLights();
		m_viewer->SetLightOn();
		m_view = m_viewer->CreateView();

		// 4. 创建交互式上下文（AIS核心）
		m_context = new AIS_InteractiveContext(m_viewer);
		m_context->SetDisplayMode(AIS_Shaded, Standard_True); // 着色模式
		m_context->SetAutomaticHilight(Standard_True);		  // 启用自动高亮

		qDebug() << "OCCT core initialized (no window yet)";
		m_initialized = true;
	}
	void View::resizeGL(int w, int h)
	{
		if (m_view.IsNull()) return;

		m_view->MustBeResized();
		// ⭐ 强制更新 OpenGL viewport
		m_view->InvalidateImmediate();
		//	m_view->Redraw();
	}

	void View::paintGL()
	{
		if (!m_initialized)
			return;

		bindWindowIfNeeded();

		if (m_view.IsNull())
			return;

		try
		{
			m_view->MustBeResized();
			m_view->Redraw();
		}
		catch (...)
		{
			qDebug() << "OCCT render exception, recovering...";
			m_windowBound = false;
		}

	}
	void View::init()
	{
		if (m_view.IsNull())
		{
			m_view = m_context->CurrentViewer()->CreateView();
		}


		if (myIs3dView)
		{
			Quantity_Color color1(Quantity_NOC_BLACK);
			Quantity_Color color2(Quantity_NOC_GRAY);
			m_view->SetBgGradientColors(color1, color2, Aspect_GFM_VER);
		}
		else
		{
			m_view->SetBackgroundColor(Quantity_TOC_RGB, 33.0 / 255.0, 40.0 / 255.0, 48.0 / 255.0);
			m_view->SetProj(V3d_Zpos);
		}

		//在屏幕左下角“贴”上一个金色的、8% 大小的、带深度遮挡的 3D 坐标轴指示器，帮助用户在旋转模型时认清方向
		m_view->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);
		m_view->MustBeResized();

		initViewCube();
		initAxis();
		initRubberBand();
		initCursors();
		setShading();
		buildSignals();
	}

	gp_Pnt View::convertScreenToAt(const std::vector<int>& aNewPos)
	{
		double XScreen = aNewPos[0] * dpr;
		double YScreen = aNewPos[1] * dpr;

		double x, y, z;
		m_view->Convert(XScreen, YScreen, x, y, z);
		gp_Pnt worldPnt = gp_Pnt(x, y, z);

		double xEye, yEye, zEye, xAt, yAt, zAt;
		m_view->Eye(xEye, yEye, zEye);
		m_view->At(xAt, yAt, zAt);
		gp_Pnt eyePoint(xEye, yEye, zEye);
		gp_Pnt atPoint(xAt, yAt, zAt);

		// gp_Dir eyeDir(gp_Vec(eyePoint , atPoint)); // 俯视图出线长度为0异常
		gp_Dir eyeDir = getV3dView()->Camera()->Direction();
		gp_Pln plnAt = gp_Pln(atPoint, eyeDir);

		// 同
		// gp_Pnt2d ConvertedPointOnPlane = ProjLib::Project(plnAt , worldPnt);
		// gp_Pnt position1 = ElSLib::Value(ConvertedPointOnPlane.X() , ConvertedPointOnPlane.Y() , plnAt);

		Handle(Geom_Plane) geoPlane = new Geom_Plane(plnAt);
		GeomAPI_ProjectPointOnSurf project(worldPnt, geoPlane);

		return project.NearestPoint();
	}

	void View::eraseAll()
	{
		m_context->EraseAll(false);
		initDisplayed();
		m_context->UpdateCurrentViewer();
	}

	void View::erase()
	{
		std::vector<Handle(AIS_InteractiveObject)> selectedObjects;
		m_context->InitSelected();
		while (m_context->MoreSelected())
		{
			Handle(AIS_InteractiveObject) aAis = m_context->SelectedInteractive();
			selectedObjects.push_back(aAis);
			m_context->NextSelected();
		}
		for (auto aAis : selectedObjects)
		{
			m_context->Erase(aAis, false);
		}

		initDisplayed();
		m_context->UpdateCurrentViewer();
	}

	void View::removeAll()
	{
		m_context->RemoveAll(false);
		initDisplayed();
		m_context->UpdateCurrentViewer();
	}

	void View::remove()
	{
		std::vector<Handle(AIS_InteractiveObject)> selectedObjects;
		m_context->InitSelected();
		while (m_context->MoreSelected())
		{
			Handle(AIS_InteractiveObject) aAis = m_context->SelectedInteractive();
			selectedObjects.push_back(aAis);
			m_context->NextSelected();
		}
		for (auto aAis : selectedObjects)
		{
			m_context->Remove(aAis, false);
		}
		initDisplayed();
		m_context->UpdateCurrentViewer();
	}

	void View::setProjectionType(Graphic3d_Camera::Projection prj)
	{
		m_view->Camera()->SetProjectionType(prj);
		m_view->Redraw();
	}

	void View::fitAll()
	{
		if (m_context->NbSelected() > 0)
		{
			m_context->FitSelected(m_view);
		}
		else
		{
			m_view->FitAll();
		}

		m_view->ZFitAll();
		m_view->Redraw();
	}

	void View::setAxo()
	{
		if (myIs3dView)
		{
			m_view->SetProj(V3d_XposYnegZpos);
		}
	}

	void View::setFront()
	{
		m_view->SetProj(V3d_Yneg);
	}

	void View::setBack()
	{
		m_view->SetProj(V3d_Ypos);
	}

	void View::setLeft()
	{
		m_view->SetProj(V3d_Xneg);
	}

	void View::setRight()
	{
		m_view->SetProj(V3d_Xpos);
	}

	void View::setTop()
	{
		m_view->SetProj(V3d_Zpos);
	}

	void View::setBottom()
	{
		m_view->SetProj(V3d_Zneg);
	}

	void View::setShading()
	{
		m_context->InitSelected();
		while (m_context->MoreSelected())
		{
			Handle(AIS_InteractiveObject) aAis = m_context->SelectedInteractive();
			m_context->SetDisplayMode(aAis, 1, false);
			m_context->NextSelected();
		}

		m_context->UpdateCurrentViewer();
	}

	void View::setWireframe()
	{
		m_context->InitSelected();
		while (m_context->MoreSelected())
		{
			Handle(AIS_InteractiveObject) aAis = m_context->SelectedInteractive();
			m_context->SetDisplayMode(aAis, 0, false);

			m_context->NextSelected();
		}

		m_context->UpdateCurrentViewer();
	}

	void View::setHLR(bool theState)
	{
		QApplication::setOverrideCursor(Qt::WaitCursor);
		m_view->SetComputedMode(theState);
		m_view->Redraw();

		QApplication::restoreOverrideCursor();
	}

	void View::setRaytracing(bool theState)
	{
		QApplication::setOverrideCursor(Qt::WaitCursor);
		if (theState)
		{
			m_view->ChangeRenderingParams().Method = Graphic3d_RM_RAYTRACING;
		}
		else
		{
			m_view->ChangeRenderingParams().Method = Graphic3d_RM_RASTERIZATION;
		}
		myIsRaytracing = theState;
		m_context->UpdateCurrentViewer();
		QApplication::restoreOverrideCursor();
	}

	void View::SetRaytracedShadows(bool theState)
	{
		m_view->ChangeRenderingParams().IsShadowEnabled = theState;
		myIsShadowsEnabled = theState;
		m_context->UpdateCurrentViewer();
	}

	void View::SetRaytracedReflections(bool theState)
	{
		m_view->ChangeRenderingParams().IsReflectionEnabled = theState;
		myIsReflectionsEnabled = theState;
		m_context->UpdateCurrentViewer();
	}

	void View::SetRaytracedAntialiasing(bool theState)
	{
		m_view->ChangeRenderingParams().IsAntialiasingEnabled = theState;
		myIsAntialiasingEnabled = theState;
		m_context->UpdateCurrentViewer();
	}

	void View::setBackgroundColor(QColor aRetColor)
	{
		if (aRetColor.isValid())
		{
			m_view->SetBgGradientStyle(Aspect_GFM_NONE);
			m_view->SetBackgroundColor(Quantity_TOC_RGB, aRetColor.redF(), aRetColor.greenF(), aRetColor.blueF());
			m_view->Redraw();
		}
	}

	void View::setEnvironmentMap(const QString& fileName)
	{
		if (!fileName.isEmpty())
		{
			Handle(Graphic3d_TextureEnv) aTexture = new Graphic3d_TextureEnv(fileName.toStdString().c_str());
			m_view->SetTextureEnv(aTexture);
		}
		else
		{
			m_view->SetTextureEnv(Handle(Graphic3d_TextureEnv)());
		}

		m_view->Redraw();
	}

	void View::setTransparency(double aTranspValue)
	{
		NCollection_List<Handle(AIS_InteractiveObject)> anAisObjectsList;
		m_context->DisplayedObjects(anAisObjectsList);
		if (anAisObjectsList.Extent() == 0)
		{
			return;
		}
		for (NCollection_List<Handle(AIS_InteractiveObject)>::Iterator anIter(anAisObjectsList); anIter.More(); anIter.Next())
		{
			const Handle(AIS_InteractiveObject)& anAisObject = anIter.Value();
			m_context->SetTransparency(anAisObject, aTranspValue, Standard_False);
		}
		m_context->UpdateCurrentViewer();
	}

	QPaintEngine* View::paintEngine() const
	{
		return nullptr;
	}

	void View::showEvent(QShowEvent* event)
	{
		//QWidget::showEvent(event);

		//// 窗口已经映射，取当前 QWindow 所在屏幕的 DPR
		//QWindow* win = window()->windowHandle();
		//QScreen* scr = win ? win->screen() : nullptr;
		//const qreal newDpr = scr ? scr->devicePixelRatio() : devicePixelRatioF();

		//if (!qFuzzyCompare(dpr, newDpr))
		//{
		//	dpr = newDpr;
		//	//Handle(Aspect_NeutralWindow) hWnd = Handle(Aspect_NeutralWindow)::DownCast(m_view->Window());
		//	//if (!hWnd.IsNull())
		//	//{
		//	//	hWnd->setDPR(dpr);
		//	//}
		//}
		//if (!m_isInitialized && winId() != 0)
		//{
		//	// ...[您的OCCT初始化代码]...

		//	m_isInitialized = true;

		//	// 关键：初始化完成后立即发出信号！
		//	emit initialized(); // ← 这行不能漏！
		//}
		bindWindowIfNeeded();
		update();
	}

	void View::resizeEvent(QResizeEvent* event)
	{
		if (!m_view.IsNull())
		{
			m_view->MustBeResized();
		}
	}

	void View::mousePressEvent(QMouseEvent* theEvent)
	{
		myClickPos = NCollection_Vec2<int>(dpr * theEvent->pos().x(), dpr * theEvent->pos().y());

		if (!m_view.IsNull())
		{
			if (auto* tool =
				ToolManager::Instance().Current())
			{
				tool->MousePress(theEvent);
			}

			if (theEvent->button() == Qt::MiddleButton && theEvent->modifiers() & Qt::ShiftModifier)
			{
				if (myIs3dView)
				{
					myCurrentMode = CurrentAction3d_DynamicRotation;
					m_view->StartRotation(myClickPos.x(), myClickPos.y());
				}
			}
			else if (theEvent->button() == Qt::MiddleButton)
			{
				myCurrentMode = CurrentAction3d_DynamicPanning;
			}

			activateCursor(myCurrentMode);
			update();
		}
	}

	void View::mouseReleaseEvent(QMouseEvent* theEvent)
	{
		const NCollection_Vec2<int> aNewPos = NCollection_Vec2<int>(dpr * theEvent->pos().x(), dpr * theEvent->pos().y());
		if (!m_view.IsNull())
		{
			myCurrentMode = CurrentAction3d_Nothing;
			activateCursor(myCurrentMode);
			bool hasRubberBand = m_context->IsDisplayed(myRubberBand);
			if (theEvent->button() == Qt::LeftButton && !hasRubberBand)
			{
				if (theEvent->modifiers() & Qt::ControlModifier)
					m_context->SelectDetected(AIS_SelectionScheme_XOR);
				else if (theEvent->modifiers() & Qt::ShiftModifier)
				{
					m_context->SelectDetected(AIS_SelectionScheme_Add);
				}
				else
				{
					m_context->SelectDetected(); // 此代码没有viewcube无法旋转
				}
			}
			else if (theEvent->button() == Qt::LeftButton && hasRubberBand)
			{
				if (theEvent->modifiers() & Qt::ControlModifier)
				{
					m_context->SelectRectangle(myClickPos, aNewPos, m_view, AIS_SelectionScheme_XOR);
				}
				else if (theEvent->modifiers() & Qt::ShiftModifier)
				{
					m_context->SelectRectangle(myClickPos, aNewPos, m_view, AIS_SelectionScheme_Add);
				}
				else if (!(theEvent->modifiers() & (Qt::ShiftModifier | Qt::ControlModifier)))
				{
					m_context->SelectRectangle(myClickPos, aNewPos, m_view);
				}
				m_context->Remove(myRubberBand, false);
			}

			myClickPos = aNewPos;

			update();
			emit selectionChanged();
		}
	}

	void View::mouseMoveEvent(QMouseEvent* theEvent)
	{
		const NCollection_Vec2<int> aNewPos(dpr * theEvent->pos().x(), dpr * theEvent->pos().y());
		if (!m_view.IsNull())
		{
			m_context->MoveTo(aNewPos.x(), aNewPos.y(), m_view, false);
			if (auto* tool =
				ToolManager::Instance().Current())
			{
				tool->MouseMove(theEvent);
			}
			if (theEvent->buttons() & Qt::LeftButton)
			{
				myRubberBand->SetRectangle(myClickPos.x(), height() * dpr - myClickPos.y(),
					aNewPos.x(), height() * dpr - aNewPos.y());
				if (m_context->IsDisplayed(myRubberBand))
					m_context->Redisplay(myRubberBand, false);
				else
					m_context->Display(myRubberBand, false);
			}
			else
			{
				switch (myCurrentMode)
				{
				case CurrentAction3d_Nothing:
					break;
				case CurrentAction3d_DynamicZooming:
					myCurrentMode = CurrentAction3d_Nothing;
					break;
				case CurrentAction3d_DynamicPanning:
					m_view->Pan(aNewPos.x() - myClickPos.x(), myClickPos.y() - aNewPos.y());
					myClickPos = aNewPos;
					break;
				case CurrentAction3d_DynamicRotation:
					if (myIs3dView)
					{
						m_view->Rotation(aNewPos.x(), aNewPos.y());
					}
					break;
				case CurrentAction3d_ObjectDececting:
					break;
				default:
					break;
				}
				activateCursor(myCurrentMode);
			}
			update();
		}
	}

	void View::wheelEvent(QWheelEvent* theEvent)
	{
		myClickPos = NCollection_Vec2<int>(dpr * theEvent->position().x(), dpr * theEvent->position().y());
		if (!m_view.IsNull())
		{
			myCurrentMode = CurrentAction3d_DynamicZooming;

			m_view->StartZoomAtPoint(myClickPos.x(), myClickPos.y());
			m_view->ZoomAtPoint(0, 0, theEvent->angleDelta().y() / 8, 0);

			activateCursor(myCurrentMode);
			update();
		}
	}

	void View::initDisplayed(bool theUpdate)
	{
		if (!m_context->IsDisplayed(myViewCube))
		{
			m_context->Display(myViewCube, theUpdate);
		}
		if (!m_context->IsDisplayed(myTrihedron))
		{
			m_context->Display(myTrihedron, theUpdate);
		}
	}

	void View::initViewCube()
	{
		myViewCube = new AIS_ViewCube();
		// myViewCube->SetBoxTransparency(0.5);
		Handle(Prs3d_Drawer) myDrawer = myViewCube->Attributes();
		Handle(Prs3d_DatumAspect) datumAspect = new Prs3d_DatumAspect();
		datumAspect->ShadingAspect(Prs3d_DatumParts_XAxis)->SetColor(Quantity_NOC_RED);
		datumAspect->ShadingAspect(Prs3d_DatumParts_YAxis)->SetColor(Quantity_NOC_GREEN);
		datumAspect->ShadingAspect(Prs3d_DatumParts_ZAxis)->SetColor(Quantity_NOC_BLUE);
		datumAspect->TextAspect(Prs3d_DatumParts_XAxis)->SetColor(Quantity_NOC_RED);
		datumAspect->TextAspect(Prs3d_DatumParts_YAxis)->SetColor(Quantity_NOC_GREEN);
		datumAspect->TextAspect(Prs3d_DatumParts_ZAxis)->SetColor(Quantity_NOC_BLUE);
		myDrawer->SetDatumAspect(datumAspect);
		myViewCube->SetTransformPersistence(new Graphic3d_TransformPers(Graphic3d_TMF_TriedronPers, Aspect_TOTP_RIGHT_UPPER, NCollection_Vec2<int>(100, 100)));
		m_context->Display(myViewCube, false);
	}

	void View::initAxis()
	{
		gp_Ax2 ax2 = gp_Ax2(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1));
		Handle(Geom_Axis2Placement) aGeoAxis2Placement = new Geom_Axis2Placement(ax2);
		myTrihedron = new AIS_Trihedron(aGeoAxis2Placement);

		myTrihedron->SetDatumPartColor(Prs3d_DatumParts_XArrow, Quantity_NOC_RED);
		myTrihedron->SetDatumPartColor(Prs3d_DatumParts_YArrow, Quantity_NOC_GREEN);
		myTrihedron->SetDatumPartColor(Prs3d_DatumParts_ZArrow, Quantity_NOC_BLUE);
		myTrihedron->SetDatumPartColor(Prs3d_DatumParts_XAxis, Quantity_NOC_RED);
		myTrihedron->SetDatumPartColor(Prs3d_DatumParts_YAxis, Quantity_NOC_GREEN);
		myTrihedron->SetDatumPartColor(Prs3d_DatumParts_ZAxis, Quantity_NOC_BLUE);
		myTrihedron->SetTextColor(Prs3d_DatumParts_XAxis, Quantity_NOC_RED);
		myTrihedron->SetTextColor(Prs3d_DatumParts_YAxis, Quantity_NOC_GREEN);
		myTrihedron->SetTextColor(Prs3d_DatumParts_ZAxis, Quantity_NOC_BLUE);

		// 显示模式shade在此设置，否则display方法报错
		myTrihedron->SetDatumDisplayMode(Prs3d_DM_Shaded);
		myTrihedron->SetDrawArrows(true);
		Handle(Graphic3d_TransformPers) aPers = new Graphic3d_TransformPers(Graphic3d_TMF_ZoomPers, gp_Pnt(0.0, 0.0, 0.0));
		myTrihedron->SetTransformPersistence(aPers);

		m_context->Display(myTrihedron, 0, AIS_TrihedronSelectionMode_Axes, false);
	}

	void View::initRubberBand()
	{
		myRubberBand = new AIS_RubberBand();
		myRubberBand->SetLineType(Aspect_TOL_DASH);
		myRubberBand->SetLineColor(Quantity_NOC_WHITE);
	}

	void View::buildSignals()
	{
		// 屏幕切换时更新 dpr（如仍需缓存）
		connect(window()->windowHandle(), &QWindow::screenChanged, this, [this](QScreen* s)
			{

				this->dpr = s ? s->devicePixelRatio() : this->devicePixelRatioF();
				// 同步 OcctWindow 的 DPR 并触发视图重算尺寸
				Handle(OcctWindow) hWnd = Handle(OcctWindow)::DownCast(m_view->Window());
				if (!hWnd.IsNull())
				{
					hWnd->setDPR(this->dpr);
				} });
	}

	void View::initCursors()
	{
		if (!defCursor)
			defCursor = new QCursor(Qt::ArrowCursor);
		if (!handCursor)
			handCursor = new QCursor(Qt::PointingHandCursor);
		if (!panCursor)
			panCursor = new QCursor(Qt::SizeAllCursor);
		if (!globPanCursor)
			globPanCursor = new QCursor(Qt::CrossCursor);
		if (!zoomCursor)
			zoomCursor = new QCursor(QPixmap(":/icons/icons/cursor_zoom.png"));
		if (!rotCursor)
			rotCursor = new QCursor(QPixmap(":/icons/icons/cursor_rotate.png"));
	}

	void View::activateCursor(const CurrentAction3d theMode)
	{
		QCursor* aCursor = defCursor;
		switch (theMode)
		{
		case CurrentAction3d_DynamicPanning:
			aCursor = panCursor;
			break;
		case CurrentAction3d_DynamicZooming:
			aCursor = zoomCursor;
			break;
		case CurrentAction3d_DynamicRotation:
			aCursor = rotCursor;
			break;
		case CurrentAction3d_GlobalPanning:
			aCursor = globPanCursor;
			break;
		case CurrentAction3d_WindowZooming:
			aCursor = handCursor;
			break;
		case CurrentAction3d_Nothing:
			aCursor = defCursor;
			break;
		default:
			break;
		}
		setCursor(*aCursor);
	}
	void View::bindWindowIfNeeded()
	{
		if (m_windowBound || m_view.IsNull())
			return;

		WId win = (WId)this->winId();
		Handle(WNT_Window) window = new WNT_Window((Aspect_Handle)win);

		m_view->SetWindow(window);

		if (!window->IsMapped())
			window->Map();

		m_view->MustBeResized();

		m_windowBound = true;
		qDebug() << "OCCT window bound (stable mode)";
		if (!m_ready)
		{
			init();
			m_ready = true;

			// 订阅 Document ChangeSet（CadObject 变更）
			App::Instance().eventBus().subscribe("document.changed",
				[this](const std::any& data) {
					auto* cs = std::any_cast<ChangeSet>(&data);
					if (!cs || cs->empty()) return;
					for (auto* o : cs->added)
						addCadAIS(o);
					for (auto* o : cs->modified)
						updateCadAIS(o);
					for (auto* o : cs->removed)
						removeCadAIS(o);
					m_context->UpdateCurrentViewer();
				});

			emit viewReady();
		}

	}

	// ============================================================
	// DocumentObserver 实现
	// ============================================================

	void View::onObjectAdded(int id, const TopoDS_Shape& /*shape*/)
	{
		if (!m_doc) return;
		auto* labelPtr = static_cast<const TDF_Label*>(m_doc->labelById(id));
		if (!labelPtr) return;

		auto ais = new XCAFPrs_AISObject(*labelPtr);
		m_context->Display(ais, Standard_True);
		m_aisMap[id] = ais;
	}

	void View::onObjectRemoved(int id)
	{
		auto it = m_aisMap.find(id);
		if (it != m_aisMap.end())
		{
			m_context->Remove(it->second, Standard_False);
			m_context->UpdateCurrentViewer();
			m_aisMap.erase(it);
		}
	}

	void View::selectObjectById(int id)
	{
		auto it = m_aisMap.find(id);
		if (it == m_aisMap.end()) return;

		m_context->ClearSelected(Standard_False);
		m_context->SetSelected(it->second, Standard_True);
		m_context->UpdateCurrentViewer();
	}

	std::vector<int> View::selectedObjectIds() const
	{
		std::vector<int> ids;
		m_context->InitSelected();
		while (m_context->MoreSelected())
		{
			auto ais = m_context->SelectedInteractive();
			for (auto& [id, mapped] : m_aisMap)
				if (mapped == ais) { ids.push_back(id); break; }
			m_context->NextSelected();
		}
		return ids;
	}

	// ============================================================
	// CadObject AIS 管线（EventBus "document.changed" 驱动）
	// ============================================================

	void View::addCadAIS(CadObject* obj)
	{
		if (!obj || obj->shape().IsNull()) return;
		auto ais = new AIS_Shape(obj->shape());
		m_context->Display(ais, Standard_False);
		m_cadAisMap[obj] = ais;
	}

	void View::updateCadAIS(CadObject* obj)
	{
		auto it = m_cadAisMap.find(obj);
		if (it == m_cadAisMap.end()) { addCadAIS(obj); return; }
		// 创建新的 AIS_Shape 替换
		m_context->Remove(it->second, Standard_False);
		auto ais = new AIS_Shape(obj->shape());
		m_context->Display(ais, Standard_False);
		it->second = ais;
	}

	void View::removeCadAIS(CadObject* obj)
	{
		auto it = m_cadAisMap.find(obj);
		if (it == m_cadAisMap.end()) return;
		m_context->Remove(it->second, Standard_False);
		m_cadAisMap.erase(it);
	}

	void View::selectCadObject(CadObject* obj)
	{
		auto it = m_cadAisMap.find(obj);
		if (it == m_cadAisMap.end()) return;
		m_context->ClearSelected(Standard_False);
		m_context->SetSelected(it->second, Standard_True);
		m_context->UpdateCurrentViewer();
	}

	std::vector<CadObject*> View::selectedCadObjects() const
	{
		std::vector<CadObject*> result;
		m_context->InitSelected();
		while (m_context->MoreSelected())
		{
			auto ais = m_context->SelectedInteractive();
			for (auto& [obj, mapped] : m_cadAisMap)
				if (mapped == ais) { result.push_back(obj); break; }
			m_context->NextSelected();
		}
		return result;
	}

	// ============================================================
	// 临时预览（不写入 Document）
	// ============================================================

	void View::showTemporaryShape(const TopoDS_Shape& shape)
	{
		clearTemporaryShape();
		m_tempAIS = new AIS_Shape(shape);
		m_context->Display(m_tempAIS, Standard_True);
	}

	void View::updateTemporaryShape(const TopoDS_Shape& shape)
	{
		clearTemporaryShape();
		m_tempAIS = new AIS_Shape(shape);
		m_context->Display(m_tempAIS, Standard_True);
	}

	void View::clearTemporaryShape()
	{
		if (!m_tempAIS.IsNull())
		{
			m_context->Remove(m_tempAIS, Standard_True);
			m_tempAIS.Nullify();
		}
	}

}