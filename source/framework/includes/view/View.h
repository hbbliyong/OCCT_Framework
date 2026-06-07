#pragma once

#include "core/CoreMacro.hpp"
#include "occ/OcctWindow.h"

#include <QWidget>
#include <QMouseEvent>
#include <QApplication>
#include <QPainter>
#include <QWindow>

#include "Geom_Plane.hxx"
#include "GeomAPI_ProjectPointOnSurf.hxx"
#include "Geom_Axis2Placement.hxx"

#include "OpenGl_GraphicDriver.hxx"
#include "Aspect_DisplayConnection.hxx"
#include "AIS_TrihedronSelectionMode.hxx"
#include "AIS_InteractiveContext.hxx"
#include "AIS_ViewCube.hxx"
#include "AIS_Trihedron.hxx"
#include "AIS_Shape.hxx"
#include "AIS_RubberBand.hxx"
#include "V3d_Viewer.hxx"
#include "V3d_View.hxx"

namespace SongYun {

	enum CurrentAction3d
	{
		CurrentAction3d_Nothing,
		CurrentAction3d_DynamicZooming,
		CurrentAction3d_WindowZooming,
		CurrentAction3d_DynamicPanning,
		CurrentAction3d_GlobalPanning,
		CurrentAction3d_DynamicRotation,
		CurrentAction3d_ObjectDececting
	};
	enum ViewAction
	{
		ViewAction_FitAll,
		ViewAction_FitArea,
		ViewAction_Zoom,
		ViewAction_Pan,
		ViewAction_GlobalPan,
		ViewAction_Front,
		ViewAction_Back,
		ViewAction_Top,
		ViewAction_Bottom,
		ViewAction_Left,
		ViewAction_Right,
		ViewAction_Axo,
		ViewAction_Rotation,
		ViewAction_Reset,
		ViewAction_HlrOff,
		ViewAction_HlrOn,
		ViewAction_Shading,
		ViewAction_Wireframe,
		ViewAction_Transparency
	};
	enum RaytraceAction
	{
		RaytraceAction_Raytracing,
		RaytraceAction_Shadows,
		RaytraceAction_Reflections,
		RaytraceAction_Antialiasing
	};

	class View : public QWidget
	{
		Q_OBJECT

	signals:
		void selectionChanged();

	public:
		View(const Handle(AIS_InteractiveContext)& theContext, bool theIs3dView = true, QWidget* theParent = nullptr);

		/**
		 * @brief 初始化
		 */
		void init();

		/**
		 * @brief 获取dpr
		 * @return
		 */
		double getDPR() const { return dpr; }

		/**
		 * @brief 获取V3d_View
		 * @return
		 */
		const Handle(V3d_View) getV3dView() const
		{
			return myV3dView;
		}

		/**
		 * @brief 获取Context
		 * @return
		 */
		Handle(AIS_InteractiveContext)& getContext()
		{
			return myContext;
		}

		/**
		 * @brief 将屏幕点转化为at处的点
		 * @param aNewPos
		 * @return
		 */
		gp_Pnt convertScreenToAt(const std::vector<int>& aNewPos);

		/**
		 * @brief hide all
		 */
		void eraseAll();

		/**
		 * @brief hide
		 */
		void erase();

		/**
		 * @brief remove all
		 */
		void removeAll();

		/**
		 * @brief remove
		 */
		void remove();

		/**
		 * @brief 设置投影类型，默认正交
		 * @param prj
		 */
		void setProjectionType(Graphic3d_Camera::Projection prj = Graphic3d_Camera::Projection_Orthographic);

		/**
		 * @brief 自适应所有AIS
		 */
		void fitAll();
		void setAxo();
		void setFront();
		void setBack();
		void setLeft();
		void setRight();
		void setTop();
		void setBottom();

		void setShading();
		void setWireframe();

		void setHLR(bool theState);
		void setRaytracing(bool theState);
		void SetRaytracedShadows(bool theState);
		void SetRaytracedReflections(bool theState);
		void SetRaytracedAntialiasing(bool theState);

		void setBackgroundColor(QColor aRetColor);
		void setEnvironmentMap(const QString& fileName);
		void setTransparency(double aTranspValue);

	protected:
		virtual  QPaintEngine* paintEngine() const;

		virtual  void showEvent(QShowEvent* event) override;
		virtual  void paintEvent(QPaintEvent*) override;
		virtual  void resizeEvent(QResizeEvent*) override;
		virtual  void mousePressEvent(QMouseEvent*) override;
		virtual  void mouseReleaseEvent(QMouseEvent*) override;
		virtual  void mouseMoveEvent(QMouseEvent*) override;
		virtual  void wheelEvent(QWheelEvent*) override;

	private:
		/**
		 * @brief 检查viewcube/Axis,若没显示则显示出来
		 */
		void initDisplayed(bool theUpdate = false);

		/**
		 * @brief 初始化ViewCube
		 */
		void initViewCube();

		/**
		 * @brief 原点处设置transformPersistence坐标轴
		 */
		void initAxis();

		/**
		 * @brief 初始化矩形选择框
		 */
		void initRubberBand();

		void buildSignals();
		void initCursors();
		void activateCursor(const CurrentAction3d);

		bool myIs3dView;
		double dpr;
		Handle(V3d_View) myV3dView;
		Handle(AIS_InteractiveContext) myContext;
		Handle(AIS_ViewCube) myViewCube;
		Handle(AIS_Trihedron) myTrihedron;
		Handle(AIS_RubberBand) myRubberBand;

		CurrentAction3d myCurrentMode;
		NCollection_Vec2<int> myClickPos;

		bool myIsRaytracing;
		bool myIsShadowsEnabled;
		bool myIsReflectionsEnabled;
		bool myIsAntialiasingEnabled;
	};

} // namespace SongYun
