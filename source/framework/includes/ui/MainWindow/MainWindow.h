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
#include "view/View.h"

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

		void initOCCTView();
		void initMenuBar();
		void initStatusBar();
		void initConnections();
		void initToolBar();


		void updateStatus(const QString& message);

		bool eventFilter(QObject* obj, QEvent* event) override;
		void onViewMouseRelease(QMouseEvent* event);

	private:

		void updateStatusBar(const std::vector<int>& aNewPos);

		std::vector<int> myClickPos;
		std::vector<Handle(AIS_InteractiveObject)> objs;

		View* view_occt;
		CommandManager myCommandManager;

		QMenuBar* myMenuBar;
		QToolBar* myToolBar;
	};

} // namespace SongYun
