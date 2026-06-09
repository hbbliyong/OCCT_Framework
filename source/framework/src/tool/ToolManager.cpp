#include "tool/ToolManager.h"
#include <QMouseEvent>
namespace SongYun
{
	void ToolManager::SetCurrent(
		std::shared_ptr<Tool> tool)
	{
		if (m_current)
			m_current->OnLeave();

		m_current = std::move(tool);

		if (m_current)
			m_current->OnEnter();
	}

	Tool *ToolManager::Current() const
	{
		return m_current.get();
	}
}