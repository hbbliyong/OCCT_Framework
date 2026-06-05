#pragma once
#include <QOpenGLWidget>
#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>
#include <OpenGl_GraphicDriver.hxx>

class OccView : public QOpenGLWidget {
	Q_OBJECT
public:
	OccView(QWidget* parent = nullptr);
	~OccView() = default;

	void displayBox(); // 示例：显示立方体
	void displaySurface(); // 示例：显示曲面
signals:
	void initialized(); // ← 核心：必须声明此信号
protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int w, int h) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;
	void showEvent(QShowEvent* event) override;


private:
	Handle(V3d_Viewer) m_viewer;
	Handle(V3d_View) m_view;
	Handle(AIS_InteractiveContext) m_context;
	bool m_isInitialized = false;
};