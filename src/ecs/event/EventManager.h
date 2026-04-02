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
    void emit(const BaseEvent &event) const {
        for (const auto &listener: listeners) {
            listener(event);
        }
    }

    void subscribe(const Handler &callback) {
        listeners.emplace_back(callback);
    }

private:
    //each event type essentially has its own std::vector of listeners without you having to manage it explicitly. this is done using the static local
    std::vector<Handler> listeners;
};
#endif //INC_8051TUTORIAL_EVENTMANAGER_H
