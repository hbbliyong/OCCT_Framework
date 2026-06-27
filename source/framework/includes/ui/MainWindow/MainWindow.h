#pragma once

#include "core/CoreMacro.hpp"
#include <memory>
#include <QEvent>
#include <QMainWindow>
#include "view/View.h"

class QMenuBar;
class QToolBar;

namespace SongYun {

	class CommandManager;
	class ActionManager;

	class MainWindow : public QMainWindow
	{
		Q_OBJECT

	public:
		SONGYUN_API static MainWindow& Instance();

		SONGYUN_API QMenuBar* menuBar() const;
		SONGYUN_API QToolBar* toolBar() const;
		SONGYUN_API ActionManager& actionManager();

		SONGYUN_API void close();

	private:
		explicit MainWindow(QWidget* parent = nullptr);
		~MainWindow() = default;
		MainWindow(const MainWindow&) = delete;
		MainWindow& operator=(const MainWindow&) = delete;

		void initOCCTView();
		void initMenuBar();
		void initStatusBar();
		void initConnections();
		void initToolBar();
		bool eventFilter(QObject* obj, QEvent* event) override;

		std::unique_ptr<ActionManager>  m_actionManager;

		QMenuBar* myMenuBar = nullptr;
		QToolBar* myToolBar = nullptr;
		View* view_occt = nullptr;
	};

} // namespace SongYun
