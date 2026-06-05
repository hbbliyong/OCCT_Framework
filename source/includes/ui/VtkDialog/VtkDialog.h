#pragma once
#include <QDialog>

#include <vtkSmartPointer.h>
class QVTKOpenGLNativeWidget;
class vtkGenericOpenGLRenderWindow;
class vtkRenderer;
class VtkDialog : public QDialog {
	Q_OBJECT

public:
	explicit VtkDialog(QWidget* parent = nullptr);
	~VtkDialog();

private:
	void setupVtkWidget(QVTKOpenGLNativeWidget* vtkWidget);
private:
	QVTKOpenGLNativeWidget* vtkWidget;
	vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
	vtkSmartPointer<vtkRenderer> renderer;
};