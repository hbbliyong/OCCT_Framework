#pragma once

#include "core/CoreMacro.hpp"
#include <any>
#include <functional>
#include <map>
#include <mutex>
#include <string>
#include <vector>

namespace SongYun {

class EventBus
{
public:
    using EventCallback = std::function<void(const std::any &)>;

    SONGYUN_API int subscribe(const std::string &topic, EventCallback callback);
    SONGYUN_API void unsubscribe(int subscriptionId);
    SONGYUN_API void publish(const std::string &topic, const std::any &data = {});

private:
    struct Subscriber
    {
        int id;
        EventCallback callback;
    };

    std::mutex m_mutex;
    std::map<std::string, std::vector<Subscriber>> m_subscribers;
    int m_nextId = 1;
};

} // namespace SongYun
