#pragma once
#include "core/CoreMacro.hpp"
#include <QDialog>

#include <vtkSmartPointer.h>
class QVTKOpenGLNativeWidget;
class vtkGenericOpenGLRenderWindow;
class vtkRenderer;

namespace SongYun {

class VtkDialog : public QDialog {
	Q_OBJECT

public:
	SONGYUN_API explicit VtkDialog(QWidget* parent = nullptr);
	SONGYUN_API ~VtkDialog();

private:
	SONGYUN_API void setupVtkWidget(QVTKOpenGLNativeWidget* vtkWidget);
private:
	QVTKOpenGLNativeWidget* vtkWidget;
	vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
	vtkSmartPointer<vtkRenderer> renderer;
};

} // namespace SongYun
