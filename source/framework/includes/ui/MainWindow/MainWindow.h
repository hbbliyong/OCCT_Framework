#pragma once

#include "core/CoreMacro.hpp"
#include <memory>
#include <functional>
#include <QEvent>
#include <QMainWindow>
#include <AIS_InteractiveContext.hxx>
#include <V3d_Viewer.hxx>
#include "command/CommandManager.h"
#include "command/ICommand.h"
#include "view/occView.h"

class QMenuBar;
class QToolBar;

namespace SongYun {

	class MainWindow : public QMainWindow
	{
		Q_OBJECT

	public:
		// 单例访问方法
		SONGYUN_API static MainWindow& Instance();

		// 获取菜单栏和工具栏的方法
		SONGYUN_API QMenuBar* menuBar() const;
		SONGYUN_API QToolBar* toolBar() const;

		SONGYUN_API void close();

	private:
		// 私有构造函数（单例模式）
		explicit MainWindow(QWidget* parent = nullptr);
		SONGYUN_API ~MainWindow() = default;

		// 禁止拷贝和赋值
		MainWindow(const MainWindow&) = delete;
		MainWindow& operator=(const MainWindow&) = delete;

		SONGYUN_API void initOCCTView();
		SONGYUN_API void initMenuBar();
		SONGYUN_API void initStatusBar();
		SONGYUN_API void initConnections();
		SONGYUN_API void initToolBar();

		SONGYUN_API void runModelData();
		SONGYUN_API void processSelectChanged();
		SONGYUN_API void displayCustormSurface();
		SONGYUN_API void setCurrentCommand(const std::shared_ptr<ICommand>& command);
		SONGYUN_API void updateStatus(const QString& message);

		SONGYUN_API bool eventFilter(QObject* obj, QEvent* event) override;
		SONGYUN_API void onViewMouseRelease(QMouseEvent* event);

	private:
		SONGYUN_API Handle(V3d_Viewer) Viewer(const V3d_TypeOfOrientation theViewProj);

		SONGYUN_API void updateStatusBar(const std::vector<int>& aNewPos);

		Handle(V3d_Viewer) myViewer;
		Handle(AIS_InteractiveContext) myContext;
		std::vector<int> myClickPos;
		std::vector<Handle(AIS_InteractiveObject)> objs;

		OccView* view_occt;
		CommandManager myCommandManager;
		std::shared_ptr<ICommand> myCurrentCommand;

		QMenuBar* myMenuBar;
		QToolBar* myToolBar;
	};

} // namespace SongYun
