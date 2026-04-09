//
// Created by taylo on 4/1/2026.
//

#ifndef INC_8051TUTORIAL_AUDIOEVENTQUEUE_H
#define INC_8051TUTORIAL_AUDIOEVENTQUEUE_H
#include <memory>
#include <queue>
#include <string>

#include "AudioManager.h"

enum class AudioCommand {
    PlayOneShot,
    StartSpawnLoop,
    StopSpawnLoop
};

class AudioEvent {
public:
    explicit AudioEvent(std::string name, AudioCommand command) : name(name), command(command) {
    }

    void execute() const {
        //allows looping of sound effect or single play
        switch (command) {
            case AudioCommand::PlayOneShot:
                AudioManager::playSfx(name);
                break;
            case AudioCommand::StartSpawnLoop:
                AudioManager::playSpawn(name);
                break;
            case AudioCommand::StopSpawnLoop:
                AudioManager::stopSpawn();
                break;
        }
    }

private:
    std::string name;
    AudioCommand command;
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
