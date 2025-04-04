#pragma once
#include <functional>
#include <map>
#include <typeindex>
#include <vector>
#include "Events.hpp"

class EventBus {
private:
    std::map<std::type_index, std::vector<std::function<void(const void*)>>> subscribers;

public:
    static EventBus& get() {
        static EventBus instance;
        return instance;
    }

    template<typename EventType, typename ClassType>
    void subscribe(ClassType* owner, void (ClassType::* callback)(const EventType&)) {
        auto& callbacks = subscribers[typeid(EventType)];
        callbacks.emplace_back([owner, callback](const void* event) {
            (owner->*callback)(*static_cast<const EventType*>(event));
        });
    }

    template<typename EventType>
    void publish(const EventType& event) {
        auto it = subscribers.find(typeid(EventType));
        if (it != subscribers.end()) {
            auto callbacks = it->second;
            for (auto& callback : callbacks) {
                callback(&event);
            }
        }
    }
};