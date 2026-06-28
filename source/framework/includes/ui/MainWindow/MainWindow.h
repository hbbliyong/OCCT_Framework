#pragma once

#include "core/CoreMacro.hpp"
#include "command/ActionManager.h"
#include <memory>
#include <QEvent>
#include <QMainWindow>
#include "view/View.h"

class QMenuBar;
class QToolBar;

namespace SongYun {

	class RibbonWidget;
	class ProjectPanel;
	class Document;

	class MainWindow : public QMainWindow
	{
		Q_OBJECT

	public:
		/// 构造时绑定 Document（一对一，不可更改）
		SONGYUN_API explicit MainWindow(Document* doc, QWidget* parent = nullptr);
		~MainWindow() = default;

		SONGYUN_API QMenuBar*      menuBar() const;
		SONGYUN_API QToolBar*      toolBar() const;
		SONGYUN_API ActionManager& actionManager();
		SONGYUN_API RibbonWidget*  ribbon() const;

	private:
		void initOCCTView(Document* doc);
		void initRibbon();
		void initMenuBar();
		void initStatusBar();
		void initConnections();
		void initToolBar();
		bool eventFilter(QObject* obj, QEvent* event) override;

		std::unique_ptr<ActionManager> m_actionManager;
		RibbonWidget*  m_ribbon       = nullptr;
		ProjectPanel*  m_projectPanel = nullptr;
		QMenuBar*      m_menubar      = nullptr;
		QToolBar*      m_toolbar      = nullptr;
		View*          m_view         = nullptr;
	};

} // namespace SongYun
