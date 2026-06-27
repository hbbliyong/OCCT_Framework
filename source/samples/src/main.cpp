#include "app/App.h"
#include "ui/MainWindow/MainWindow.h"
#include "document/DocumentManager.h"
#include "setup/MenuBarSetup.h"
#include "QApplication"
int main(int argc, char* argv[])
{
	// 强制 Qt 使用桌面 OpenGL，而不是 RHI/GLES/Vulkan
	QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
	QCoreApplication::setAttribute(Qt::AA_DisableHighDpiScaling);

	// 如果是 Qt 6，通过环境变量彻底关闭 RHI 后端
	qputenv("QT_QUICK_BACKEND", "software"); // 如果有 QML
	qputenv("QSG_RHI_BACKEND", "opengl");
	qputenv("QT_OPENGL", "desktop");

	QSurfaceFormat format;
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setVersion(4, 6);   // ⭐关键改这里
	format.setDepthBufferSize(24);
	format.setStencilBufferSize(8);
	format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);

	QSurfaceFormat::setDefaultFormat(format);


	QApplication app(argc, argv);
	SongYun::App::Instance().initialize(app, "OCCT Qt Framework");

	SongYun::DocumentManager::Instance().createDocument("First Document");

	// 使用 MainWindow 单例
	auto& mainWindow = SongYun::MainWindow::Instance();
	mainWindow.setWindowTitle("SongYun Occt Framework"); // 设置窗口标题
	mainWindow.resize(800, 600);				 // 设置初始窗口大小
	mainWindow.show();

	// 统一初始化菜单栏和工具栏（复用 ActionManager 的缓存 Action）
	Samples::MenuBarSetup::initialize(mainWindow.menuBar(), mainWindow.toolBar(),
									  mainWindow.actionManager());

	int result = SongYun::App::Instance().application()->exec();
	SongYun::App::Instance().shutdown();
	return result;
}
