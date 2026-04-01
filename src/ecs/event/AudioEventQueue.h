//
// Created by taylo on 4/1/2026.
//

#ifndef INC_8051TUTORIAL_AUDIOEVENTQUEUE_H
#define INC_8051TUTORIAL_AUDIOEVENTQUEUE_H
#include <memory>
#include <queue>
#include <string>

#include "AudioManager.h"

class AudioEvent {
public:
    explicit AudioEvent(std::string name) : name(name) {
    }

    void execute() const {
        AudioManager::playSfx(name);
    }

private:
    std::string name;
};

class AudioEventQueue {
public:
    void push(std::unique_ptr<AudioEvent> audioEvent) {
        events.push(std::move(audioEvent));
    }

    void process() {
        while (!events.empty()) {
            events.front()->execute();
            events.pop();
        }
    }

private:
    std::queue<std::unique_ptr<AudioEvent> > events;
};

#endif //INC_8051TUTORIAL_AUDIOEVENTQUEUE_H
