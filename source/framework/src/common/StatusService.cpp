#include "common/StatusService.h"
#include "common/EventBus.h"

namespace SongYun {

	StatusService::StatusService(EventBus* bus) : m_bus(bus) {}

	void StatusService::showMessage(const QString& msg, StatusType type, int timeout)
	{
		if (m_bus)
			m_bus->publish("status.message", StatusMessageEvent{ type, msg, timeout });
	}

} // namespace SongYun
