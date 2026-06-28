#include "common/EventBus.h"
#include <algorithm>

namespace SongYun {

	int EventBus::subscribe(const std::string& topic, EventCallback callback)
	{
		if (!callback) return 0;

		std::lock_guard lock(m_mutex);
		int id = m_nextId++;
		m_subscribers[topic].push_back({ id, std::move(callback) });
		return id;
	}

	void EventBus::unsubscribe(int subscriptionId)
	{
		std::lock_guard lock(m_mutex);
		for (auto& [topic, list] : m_subscribers)
		{
			list.erase(std::remove_if(list.begin(), list.end(),
				[subscriptionId](const Subscriber& sub) { return sub.id == subscriptionId; }),
				list.end());
		}
	}

	void EventBus::publish(const std::string& topic, const std::any& data)
	{
		std::vector<Subscriber> listeners;
		{
			std::lock_guard lock(m_mutex);
			auto it = m_subscribers.find(topic);
			if (it == m_subscribers.end()) return;
			listeners = it->second;
		}

		for (auto& subscriber : listeners)
			if (subscriber.callback) subscriber.callback(data);
	}

} // namespace SongYun
