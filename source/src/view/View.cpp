#include "view/View.h"

static QCursor *defCursor = nullptr;
static QCursor *handCursor = nullptr;
static QCursor *panCursor = nullptr;
static QCursor *globPanCursor = nullptr;
static QCursor *zoomCursor = nullptr;
static QCursor *rotCursor = nullptr;

View::View(const Handle(AIS_InteractiveContext) & theContext, bool theIs3dView, QWidget *theParent) : QWidget(theParent),
																									  myIs3dView(theIs3dView),
																									  dpr(devicePixelRatioF()),
																									  myIsRaytracing(false),
																									  myIsShadowsEnabled(true),
																									  myIsReflectionsEnabled(false),
																									  myIsAntialiasingEnabled(false)
{
	myContext = theContext;
	myCurrentMode = CurrentAction3d_Nothing;

	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground);
	setBackgroundRole(QPalette::NoRole);
	setFocusPolicy(Qt::StrongFocus);
	setMouseTracking(true);
	init();
}

void View::init()
{
	if (myV3dView.IsNull())
	{
		myV3dView = myContext->CurrentViewer()->CreateView();
	}

	Handle(OcctWindow) hWnd = new OcctWindow(this);
	// 设置视图窗口
	myV3dView->SetWindow(hWnd);
	if (!hWnd->IsMapped())
	{
		hWnd->Map();
	}

	if (myIs3dView)
	{
		Quantity_Color color1(Quantity_NOC_BLACK);
		Quantity_Color color2(Quantity_NOC_GRAY);
		myV3dView->SetBgGradientColors(color1, color2, Aspect_GFM_VER);
	}
	else
	{
		myV3dView->SetBackgroundColor(Quantity_TOC_RGB, 33.0 / 255.0, 40.0 / 255.0, 48.0 / 255.0);
		myV3dView->SetProj(V3d_Zpos);
	}

	myV3dView->MustBeResized();

	initViewCube();
	initAxis();
	initRubberBand();
	initCursors();
	setShading();
	buildSignals();
}

gp_Pnt View::convertScreenToAt(const std::vector<int> &aNewPos)
{
	double XScreen = aNewPos[0] * dpr;
	double YScreen = aNewPos[1] * dpr;

	double x, y, z;
	myV3dView->Convert(XScreen, YScreen, x, y, z);
	gp_Pnt worldPnt = gp_Pnt(x, y, z);

	double xEye, yEye, zEye, xAt, yAt, zAt;
	myV3dView->Eye(xEye, yEye, zEye);
	myV3dView->At(xAt, yAt, zAt);
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
	myContext->EraseAll(false);
	initDisplayed();
	myContext->UpdateCurrentViewer();
}

void View::erase()
{
	std::vector<Handle(AIS_InteractiveObject)> selectedObjects;
	myContext->InitSelected();
	while (myContext->MoreSelected())
	{
		Handle(AIS_InteractiveObject) aAis = myContext->SelectedInteractive();
		selectedObjects.push_back(aAis);
		myContext->NextSelected();
	}
	for (auto aAis : selectedObjects)
	{
		myContext->Erase(aAis, false);
	}

	initDisplayed();
	myContext->UpdateCurrentViewer();
}

void View::removeAll()
{
	myContext->RemoveAll(false);
	initDisplayed();
	myContext->UpdateCurrentViewer();
}

void View::remove()
{
	std::vector<Handle(AIS_InteractiveObject)> selectedObjects;
	myContext->InitSelected();
	while (myContext->MoreSelected())
	{
		Handle(AIS_InteractiveObject) aAis = myContext->SelectedInteractive();
		selectedObjects.push_back(aAis);
		myContext->NextSelected();
	}
	for (auto aAis : selectedObjects)
	{
		myContext->Remove(aAis, false);
	}
	initDisplayed();
	myContext->UpdateCurrentViewer();
}

void View::setProjectionType(Graphic3d_Camera::Projection prj)
{
	myV3dView->Camera()->SetProjectionType(prj);
	myV3dView->Redraw();
}

void View::fitAll()
{
	if (myContext->NbSelected() > 0)
	{
		myContext->FitSelected(myV3dView);
	}
	else
	{
		myV3dView->FitAll();
	}

	myV3dView->ZFitAll();
	myV3dView->Redraw();
}

void View::setAxo()
{
	if (myIs3dView)
	{
		myV3dView->SetProj(V3d_XposYnegZpos);
	}
}

void View::setFront()
{
	myV3dView->SetProj(V3d_Yneg);
}

void View::setBack()
{
	myV3dView->SetProj(V3d_Ypos);
}

void View::setLeft()
{
	myV3dView->SetProj(V3d_Xneg);
}

void View::setRight()
{
	myV3dView->SetProj(V3d_Xpos);
}

void View::setTop()
{
	myV3dView->SetProj(V3d_Zpos);
}

void View::setBottom()
{
	myV3dView->SetProj(V3d_Zneg);
}

void View::setShading()
{
	myContext->InitSelected();
	while (myContext->MoreSelected())
	{
		Handle(AIS_InteractiveObject) aAis = myContext->SelectedInteractive();
		myContext->SetDisplayMode(aAis, 1, false);
		myContext->NextSelected();
	}

	myContext->UpdateCurrentViewer();
}

void View::setWireframe()
{
	myContext->InitSelected();
	while (myContext->MoreSelected())
	{
		Handle(AIS_InteractiveObject) aAis = myContext->SelectedInteractive();
		myContext->SetDisplayMode(aAis, 0, false);

		myContext->NextSelected();
	}

	myContext->UpdateCurrentViewer();
}

void View::setHLR(bool theState)
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	myV3dView->SetComputedMode(theState);
	myV3dView->Redraw();

	QApplication::restoreOverrideCursor();
}

void View::setRaytracing(bool theState)
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	if (theState)
	{
		myV3dView->ChangeRenderingParams().Method = Graphic3d_RM_RAYTRACING;
	}
	else
	{
		myV3dView->ChangeRenderingParams().Method = Graphic3d_RM_RASTERIZATION;
	}
	myIsRaytracing = theState;
	myContext->UpdateCurrentViewer();
	QApplication::restoreOverrideCursor();
}

void View::SetRaytracedShadows(bool theState)
{
	myV3dView->ChangeRenderingParams().IsShadowEnabled = theState;
	myIsShadowsEnabled = theState;
	myContext->UpdateCurrentViewer();
}

void View::SetRaytracedReflections(bool theState)
{
	myV3dView->ChangeRenderingParams().IsReflectionEnabled = theState;
	myIsReflectionsEnabled = theState;
	myContext->UpdateCurrentViewer();
}

void View::SetRaytracedAntialiasing(bool theState)
{
	myV3dView->ChangeRenderingParams().IsAntialiasingEnabled = theState;
	myIsAntialiasingEnabled = theState;
	myContext->UpdateCurrentViewer();
}

void View::setBackgroundColor(QColor aRetColor)
{
	if (aRetColor.isValid())
	{
		myV3dView->SetBgGradientStyle(Aspect_GFM_NONE);
		myV3dView->SetBackgroundColor(Quantity_TOC_RGB, aRetColor.redF(), aRetColor.greenF(), aRetColor.blueF());
		myV3dView->Redraw();
	}
}

void View::setEnvironmentMap(const QString &fileName)
{
	if (!fileName.isEmpty())
	{
		Handle(Graphic3d_TextureEnv) aTexture = new Graphic3d_TextureEnv(fileName.toStdString().c_str());
		myV3dView->SetTextureEnv(aTexture);
	}
	else
	{
		myV3dView->SetTextureEnv(Handle(Graphic3d_TextureEnv)());
	}

	myV3dView->Redraw();
}

void View::setTransparency(double aTranspValue)
{
	NCollection_List<Handle(AIS_InteractiveObject)> anAisObjectsList;
	myContext->DisplayedObjects(anAisObjectsList);
	if (anAisObjectsList.Extent() == 0)
	{
		return;
	}
	for (NCollection_List<Handle(AIS_InteractiveObject)>::Iterator anIter(anAisObjectsList); anIter.More(); anIter.Next())
	{
		const Handle(AIS_InteractiveObject) &anAisObject = anIter.Value();
		myContext->SetTransparency(anAisObject, aTranspValue, Standard_False);
	}
	myContext->UpdateCurrentViewer();
}

QPaintEngine *View::paintEngine() const
{
	return nullptr;
}

void View::showEvent(QShowEvent *event)
{
	QWidget::showEvent(event);

	// 窗口已经映射，取当前 QWindow 所在屏幕的 DPR
	QWindow *win = window()->windowHandle();
	QScreen *scr = win ? win->screen() : nullptr;
	const qreal newDpr = scr ? scr->devicePixelRatio() : devicePixelRatioF();

	if (!qFuzzyCompare(dpr, newDpr))
	{
		dpr = newDpr;
		Handle(OcctWindow) hWnd = Handle(OcctWindow)::DownCast(myV3dView->Window());
		if (!hWnd.IsNull())
		{
			hWnd->setDPR(dpr);
		}
	}
}

void View::paintEvent(QPaintEvent *event)
{
	myV3dView->Redraw();
}

void View::resizeEvent(QResizeEvent *event)
{
	if (!myV3dView.IsNull())
	{
		myV3dView->MustBeResized();
	}
}

void View::mousePressEvent(QMouseEvent *theEvent)
{
	myClickPos = NCollection_Vec2<int>(dpr * theEvent->pos().x(), dpr * theEvent->pos().y());

	if (!myV3dView.IsNull())
	{
		if (theEvent->button() == Qt::MiddleButton && theEvent->modifiers() & Qt::ShiftModifier)
		{
			if (myIs3dView)
			{
				myCurrentMode = CurrentAction3d_DynamicRotation;
				myV3dView->StartRotation(myClickPos.x(), myClickPos.y());
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

void View::mouseReleaseEvent(QMouseEvent *theEvent)
{
	const NCollection_Vec2<int> aNewPos = NCollection_Vec2<int>(dpr * theEvent->pos().x(), dpr * theEvent->pos().y());
	if (!myV3dView.IsNull())
	{
		myCurrentMode = CurrentAction3d_Nothing;
		activateCursor(myCurrentMode);
		bool hasRubberBand = myContext->IsDisplayed(myRubberBand);
		if (theEvent->button() == Qt::LeftButton && !hasRubberBand)
		{
			if (theEvent->modifiers() & Qt::ControlModifier)
				myContext->SelectDetected(AIS_SelectionScheme_XOR);
			else if (theEvent->modifiers() & Qt::ShiftModifier)
			{
				myContext->SelectDetected(AIS_SelectionScheme_Add);
			}
			else
			{
				myContext->SelectDetected(); // 此代码没有viewcube无法旋转
			}
		}
		else if (theEvent->button() == Qt::LeftButton && hasRubberBand)
		{
			if (theEvent->modifiers() & Qt::ControlModifier)
			{
				myContext->SelectRectangle(myClickPos, aNewPos, myV3dView, AIS_SelectionScheme_XOR);
			}
			else if (theEvent->modifiers() & Qt::ShiftModifier)
			{
				myContext->SelectRectangle(myClickPos, aNewPos, myV3dView, AIS_SelectionScheme_Add);
			}
			else if (!(theEvent->modifiers() & (Qt::ShiftModifier | Qt::ControlModifier)))
			{
				myContext->SelectRectangle(myClickPos, aNewPos, myV3dView);
			}
			myContext->Remove(myRubberBand, false);
		}

		myClickPos = aNewPos;

		update();
		emit selectionChanged();
	}
}

void View::mouseMoveEvent(QMouseEvent *theEvent)
{
	const NCollection_Vec2<int> aNewPos(dpr * theEvent->pos().x(), dpr * theEvent->pos().y());
	if (!myV3dView.IsNull())
	{
		myContext->MoveTo(aNewPos.x(), aNewPos.y(), myV3dView, false);

		if (theEvent->buttons() & Qt::LeftButton)
		{
			myRubberBand->SetRectangle(myClickPos.x(), height() * dpr - myClickPos.y(),
									   aNewPos.x(), height() * dpr - aNewPos.y());
			if (myContext->IsDisplayed(myRubberBand))
				myContext->Redisplay(myRubberBand, false);
			else
				myContext->Display(myRubberBand, false);
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
				myV3dView->Pan(aNewPos.x() - myClickPos.x(), myClickPos.y() - aNewPos.y());
				myClickPos = aNewPos;
				break;
			case CurrentAction3d_DynamicRotation:
				if (myIs3dView)
				{
					myV3dView->Rotation(aNewPos.x(), aNewPos.y());
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

void View::wheelEvent(QWheelEvent *theEvent)
{
	myClickPos = NCollection_Vec2<int>(dpr * theEvent->position().x(), dpr * theEvent->position().y());
	if (!myV3dView.IsNull())
	{
		myCurrentMode = CurrentAction3d_DynamicZooming;

		myV3dView->StartZoomAtPoint(myClickPos.x(), myClickPos.y());
		myV3dView->ZoomAtPoint(0, 0, theEvent->angleDelta().y() / 8, 0);

		activateCursor(myCurrentMode);
		update();
	}
}

void View::initDisplayed(bool theUpdate)
{
	if (!myContext->IsDisplayed(myViewCube))
	{
		myContext->Display(myViewCube, theUpdate);
	}
	if (!myContext->IsDisplayed(myTrihedron))
	{
		myContext->Display(myTrihedron, theUpdate);
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
	myContext->Display(myViewCube, false);
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

	myContext->Display(myTrihedron, 0, AIS_TrihedronSelectionMode_Axes, false);
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
	connect(window()->windowHandle(), &QWindow::screenChanged, this, [this](QScreen *s)
			{

		this->dpr = s ? s->devicePixelRatio() : this->devicePixelRatioF();
		// 同步 OcctWindow 的 DPR 并触发视图重算尺寸
		Handle(OcctWindow) hWnd = Handle(OcctWindow)::DownCast(myV3dView->Window());
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
	QCursor *aCursor = defCursor;
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