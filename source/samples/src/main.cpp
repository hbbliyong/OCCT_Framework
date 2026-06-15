#include "app/App.h"
#include "ui/MainWindow/MainWindow.h"
#include "document/DocumentManager.h"
#include "QApplication"
int main(int argc, char* argv[])
{
	QSurfaceFormat fmt = QSurfaceFormat::defaultFormat();
	fmt.setVersion(3, 3);
	fmt.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(fmt);
	QApplication app(argc, argv);
	SongYun::App::Instance().initialize(app, "OCCT Qt Framework");

	SongYun::DocumentManager::Instance().createDocument("First Document");

	// 使用 MainWindow 单例
	auto& mainWindow = SongYun::MainWindow::Instance();
	mainWindow.setWindowTitle("SongYun Occt Framework"); // 设置窗口标题
	mainWindow.resize(800, 600);				 // 设置初始窗口大小
	mainWindow.show();

	// 示例：获取菜单栏和工具栏
	auto* menuBar = mainWindow.menuBar();
	auto* toolBar = mainWindow.toolBar();

	int result = SongYun::App::Instance().application()->exec();
	SongYun::App::Instance().shutdown();
	return result;
}
