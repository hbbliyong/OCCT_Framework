#pragma once

#include <any>
#include <functional>
#include <map>
#include <mutex>
#include <string>
#include <vector>

class EventBus
{
public:
    using EventCallback = std::function<void(const std::any &)>;

    int subscribe(const std::string &topic, EventCallback callback);
    void unsubscribe(int subscriptionId);
    void publish(const std::string &topic, const std::any &data = {});

private:
    struct Subscriber
    {
        int id;
        EventCallback callback;
    };

    std::mutex mutex_;
    std::map<std::string, std::vector<Subscriber>> subscribers_;
    int nextId_ = 1;
};
