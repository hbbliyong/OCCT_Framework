#include "app/App.h"
#include "ui/MainWindow/MainWindow.h"
#include "document/ProjectManager.h"
#include "document/Project.h"
#include "setup/MenuBarSetup.h"
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char* argv[])
{
	QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
	QCoreApplication::setAttribute(Qt::AA_DisableHighDpiScaling);

	qputenv("QT_QUICK_BACKEND", "software");
	qputenv("QSG_RHI_BACKEND", "opengl");
	qputenv("QT_OPENGL", "desktop");

	QSurfaceFormat format;
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setVersion(4, 6);
	format.setDepthBufferSize(24);
	format.setStencilBufferSize(8);
	format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
	QSurfaceFormat::setDefaultFormat(format);

	QApplication app(argc, argv);
	SongYun::App::Instance().initialize(app, "OCCT Qt Framework");

	auto* project = SongYun::ProjectManager::Instance().createProject("Project001");

	// View 构造时绑定 Document（一对一，不可更改）
	auto& mainWindow = SongYun::App::Instance().mainWindow(project->document());
	mainWindow.setWindowTitle("SongYun Occt Framework");
	mainWindow.resize(800, 600);
	mainWindow.show();

	Samples::MenuBarSetup::initialize(mainWindow);

	int result = SongYun::App::Instance().application()->exec();
	SongYun::App::Instance().shutdown();
	return result;
}