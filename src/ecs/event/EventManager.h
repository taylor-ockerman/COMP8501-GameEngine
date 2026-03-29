//
// Created by taylo on 2/4/2026.
//

#ifndef INC_8051TUTORIAL_EVENTMANAGER_H
#define INC_8051TUTORIAL_EVENTMANAGER_H
#include <functional>
#include <vector>

#include "BaseEvent.h"


//Observer pattern
class EventManager {
    using Handler = std::function<void(const BaseEvent &)>;

public:
    // template<typename EventType>
    // void emit(const EventType &event) {
    //     //retrieve the list of subscribers to certain events
    //     auto &listeners = getListeners<EventType>();
    //     //loop all the subscribers to certain events
    //     for (auto listener: listeners) {
    //         listener(event); // invoke the function here
    //     }
    // }
    void emit(const BaseEvent &event) const {
        for (const auto &listener: listeners) {
            listener(event);
        }
    }

    // template<typename EventType>
    // void subscribe(std::function<void(const EventType &)> callback) {
    //     //pass in the callback/callable wrapper/subscription
    //     getListeners<EventType>().push_back(callback);
    // }
    void subscribe(const Handler &callback) {
        listeners.emplace_back(callback);
    }

private:
    //each event type essentially has its own std::vector of listeners without you having to manage it explicitly. this is done using the static local
    //std::function<void(const EventType&)> is the callable wrapper: can hold anything that can be called like a function (lambda, free function, functor etc)
    // template<typename EventType>
    // std::vector<std::function<void(const EventType &)> > &getListeners() {
    //     static std::vector<std::function<void(const EventType &)> > listeners;
    //     return listeners;
    // }

    std::vector<Handler> listeners;
};
#endif //INC_8051TUTORIAL_EVENTMANAGER_H
