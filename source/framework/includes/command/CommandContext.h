#pragma once

#include "core/CoreMacro.hpp"
#include "ICommand.h"
#include <QString>
#include <map>
#include <functional>
#include <memory>

namespace SongYun {
	class DocumentManager;
	class SelectionManager;
	class ViewManager;
	class ToolManager;
	class StatusService;

	class CommandContext
	{
	public:
		CommandContext() = default;

		void setDocumentManager(DocumentManager* mgr)  { m_documentManager = mgr; }
		void setSelectionManager(SelectionManager* mgr){ m_selectionManager = mgr; }
		void setViewManager(ViewManager* mgr)          { m_viewManager = mgr; }
		void setToolManager(ToolManager* mgr)          { m_toolManager = mgr; }
		void setStatusService(StatusService* svc)      { m_statusService = svc; }

		DocumentManager&  documentManager()  const { return *m_documentManager; }
		SelectionManager& selectionManager() const { return *m_selectionManager; }
		ViewManager&      viewManager()      const { return *m_viewManager; }
		ToolManager&      toolManager()      const { return *m_toolManager; }
		StatusService&    statusService()    const { return *m_statusService; }

	private:
		DocumentManager*  m_documentManager  = nullptr;
		SelectionManager* m_selectionManager = nullptr;
		ViewManager*      m_viewManager      = nullptr;
		ToolManager*      m_toolManager      = nullptr;
		StatusService*    m_statusService    = nullptr;
	};

} // namespace SongYun
