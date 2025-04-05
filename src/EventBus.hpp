#pragma once
#include <functional>
#include <map>
#include <typeindex>
#include <vector>
#include <algorithm>
#include "Events.hpp"

class EventBus {
private:
    std::map<std::type_index, std::vector<std::pair<uintptr_t, std::function<void(const void*)>>>> subscribers;

public:
    static EventBus& get() {
        static EventBus instance;
        return instance;
    }

    template<typename EventType, typename ClassType>
    void subscribe(ClassType* owner, void (ClassType::* callback)(const EventType&)) {
        auto& callbacks = subscribers[typeid(EventType)];
        uintptr_t ownerId = reinterpret_cast<uintptr_t>(owner);
        callbacks.emplace_back(ownerId, [owner, callback](const void* event) {
            (owner->*callback)(*static_cast<const EventType*>(event));
            });
    }

    template<typename EventType, typename ClassType>
    void unsubscribe(ClassType* owner) {
        auto it = subscribers.find(typeid(EventType));
        if (it != subscribers.end()) {
            uintptr_t ownerId = reinterpret_cast<uintptr_t>(owner);
            auto& callbacks = it->second;

            callbacks.erase(
                std::remove_if(callbacks.begin(), callbacks.end(),
                    [ownerId](const auto& pair) {
                        return pair.first == ownerId;
                    }),
                callbacks.end()
            );
        }
    }

    template<typename ClassType>
    void unsubscribeAll(ClassType* owner) {
        uintptr_t ownerId = reinterpret_cast<uintptr_t>(owner);
        for (auto& [type, callbacks] : subscribers) {
            callbacks.erase(
                std::remove_if(callbacks.begin(), callbacks.end(),
                    [ownerId](const auto& pair) {
                        return pair.first == ownerId;
                    }),
                callbacks.end()
            );
        }
    }

    template<typename EventType>
    void publish(const EventType& event) {
        auto it = subscribers.find(typeid(EventType));
        if (it != subscribers.end()) {
            auto callbacks = it->second;
            for (auto& [_, callback] : callbacks) {
                callback(&event);
            }
        }
    }
};