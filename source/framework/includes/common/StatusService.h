#pragma once

#include "core/CoreMacro.hpp"
#include <QString>

namespace SongYun {

	class EventBus;

	/// 状态消息类型
	enum class StatusType { Info, Warning, Error };

	/// 状态消息事件
	struct StatusMessageEvent
	{
		StatusType type = StatusType::Info;
		QString text;
		int timeout = 0;   // 0 = 默认 5000ms
	};

	/// @brief 状态消息服务 — 通过 EventBus 解耦命令与 UI
	class StatusService
	{
	public:
		SONGYUN_API explicit StatusService(EventBus* bus);

		SONGYUN_API void showMessage(const QString& msg, StatusType type = StatusType::Info, int timeout = 5000);

	private:
		EventBus* m_bus = nullptr;
	};

} // namespace SongYun
