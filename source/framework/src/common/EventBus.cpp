#include "common/EventBus.h"
#include <algorithm>
namespace SongYun {

	int EventBus::subscribe(const std::string& topic, EventCallback callback)
	{
		if (!callback)
			return 0;

		std::lock_guard lock(mutex_);
		int id = nextId_++;
		subscribers_[topic].push_back({ id, std::move(callback) });
		return id;
	}

	void EventBus::unsubscribe(int subscriptionId)
	{
		std::lock_guard lock(mutex_);
		for (auto& [topic, list] : subscribers_)
		{
			list.erase(std::remove_if(list.begin(), list.end(), [subscriptionId](const Subscriber& sub)
				{ return sub.id == subscriptionId; }),
				list.end());
		}
	}

	void EventBus::publish(const std::string& topic, const std::any& data)
	{
		std::vector<Subscriber> listeners;
		{
			std::lock_guard lock(mutex_);
			auto it = subscribers_.find(topic);
			if (it == subscribers_.end())
				return;
			listeners = it->second;
		}

		for (auto& subscriber : listeners)
		{
			if (subscriber.callback)
				subscriber.callback(data);
		}
	}
}