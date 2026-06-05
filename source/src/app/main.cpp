#include <QApplication>
#include <QSurfaceFormat>
#include "app/App.h"
#include "ui/MainWindow/MainWindow.h"

int main(int argc, char *argv[])
{
	QSurfaceFormat fmt = QSurfaceFormat::defaultFormat();
	fmt.setVersion(3, 3);
	fmt.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(fmt);
	QApplication app(argc, argv);
	App::Instance().initialize(&app, "OCCT Qt Framework");

	MainWindow window;
	window.setWindowTitle("Qt VTK Example"); // 设置窗口标题
	window.resize(800, 600);				 // 设置初始窗口大小
	window.show();

	int result = app.exec();
	App::Instance().shutdown();
	return result;
}