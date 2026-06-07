#pragma once
#include "core/CoreMacro.hpp"
#include <QOpenGLWidget>
#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>
#include <OpenGl_GraphicDriver.hxx>

namespace SongYun {

class OccView : public QOpenGLWidget {
	Q_OBJECT
public:
	SONGYUN_API OccView(QWidget* parent = nullptr);
	SONGYUN_API ~OccView() = default;

	SONGYUN_API void displayBox(); // 示例：显示立方体
	SONGYUN_API void displaySurface(); // 示例：显示曲面
signals:
	void initialized(); // ← 核心：必须声明此信号
protected:
	SONGYUN_API void initializeGL() override;
	SONGYUN_API void paintGL() override;
	SONGYUN_API void resizeGL(int w, int h) override;
	SONGYUN_API void mousePressEvent(QMouseEvent* event) override;
	SONGYUN_API void mouseMoveEvent(QMouseEvent* event) override;
	SONGYUN_API void wheelEvent(QWheelEvent* event) override;
	SONGYUN_API void showEvent(QShowEvent* event) override;


private:
	Handle(V3d_Viewer) m_viewer;
	Handle(V3d_View) m_view;
	Handle(AIS_InteractiveContext) m_context;
	bool m_isInitialized = false;
};

} // namespace SongYun
