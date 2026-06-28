#include "command/CommandManager.h"
#include "command/CommandRegistry.h"
#include "command/Transaction.h"
#include "document/Document.h"
#include "document/DocumentManager.h"
#include "document/ProjectManager.h"
#include "document/Project.h"
#include "app/App.h"
#include <QEventLoop>
#include <QToolBar>
#include <iostream>

namespace SongYun {

	void CommandManager::setActiveLoop(QEventLoop* loop) { m_activeLoop = loop; }

	void CommandManager::interruptActive()
	{
		if (m_activeLoop && m_activeLoop->isRunning())
			m_activeLoop->quit();
		m_activeLoop = nullptr;
		m_activeCommand = nullptr;
	}

	bool CommandManager::isExecuting() const { return m_activeCommand != nullptr; }

	Document* CommandManager::activeDocument() const
	{
		// 优先取 ProjectManager 的项目文档
		auto* proj = ProjectManager::Instance().activeProject();
		if (proj) return proj->document();
		// 回退到旧的 DocumentManager
		return DocumentManager::Instance().activeDocument().get();
	}

	void CommandManager::setParameterToolBar(QToolBar* toolbar) { m_paramToolbar = toolbar; }

	void CommandManager::buildCommandUI(ICommand* command)
	{
		if (!m_paramToolbar) return;
		CommandUI ui;
		command->buildUI(ui);
		auto* panel = m_builder.build(ui, m_paramToolbar);
		if (panel) m_paramToolbar->addWidget(panel);
	}

	void CommandManager::clearCommandUI()
	{
		m_builder.clear();
		if (m_paramToolbar)
		{
			// 移除除第一个持久 widget 外的所有内容
			while (m_paramToolbar->actions().size() > 0)
				m_paramToolbar->removeAction(m_paramToolbar->actions().last());
		}
	}

	// ============================================================
	// execute — 核心：Transaction 包裹 command
	// ============================================================

	void CommandManager::execute(ICommand* command)
	{
		if (!command) return;

		if (m_activeCommand)
			interruptActive();

		command->setContext(&App::Instance().commandContext());
		m_activeCommand = command;

		auto* doc = activeDocument();
		if (!doc) { m_activeCommand = nullptr; return; }

		// 0. 构建参数 UI
		buildCommandUI(command);

		// 1. 开启事务
		auto txn = std::make_unique<Transaction>(doc);
		doc->setActiveTransaction(txn.get());

		// 2. 执行命令
		bool ok = command->execute();

		// 3. 关闭事务记录
		doc->setActiveTransaction(nullptr);

		// 4. 如果成功 → 记录事务，清空 redo
		if (ok && !txn->empty())
		{
			m_redoStack.clear();
			m_undoStack.push_back(std::move(txn));
		}

		// 5. 清理参数 UI
		clearCommandUI();

		m_activeCommand = nullptr;
		m_activeLoop = nullptr;
	}

	void CommandManager::execute(const std::string& commandId)
	{
		auto* cmd = CommandRegistry::Instance().getCommand(commandId.c_str());
		std::cout << "Executing command: " << commandId << std::endl;
		if (cmd) execute(cmd);
	}

	// ============================================================
	// undo / redo — Transaction 重放
	// ============================================================

	void CommandManager::undo()
	{
		if (!canUndo()) return;

		auto* doc = activeDocument();

		auto txn = std::move(m_undoStack.back());
		m_undoStack.pop_back();

		doc->setActiveTransaction(nullptr);  // undo 内部操作不记录新事务
		txn->undo();

		m_redoStack.push_back(std::move(txn));
	}

	void CommandManager::redo()
	{
		if (!canRedo()) return;

		auto* doc = activeDocument();

		auto txn = std::move(m_redoStack.back());
		m_redoStack.pop_back();

		doc->setActiveTransaction(nullptr);
		txn->redo();

		m_undoStack.push_back(std::move(txn));
	}

	bool CommandManager::canUndo() const { return !m_undoStack.empty(); }
	bool CommandManager::canRedo() const { return !m_redoStack.empty(); }

	void CommandManager::clear()
	{
		m_undoStack.clear();
		m_redoStack.clear();
	}

} // namespace SongYun
