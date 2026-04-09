//
// Created by taylo on 4/1/2026.
//
#include "AudioManager.h"

#include <iostream>
#include <ostream>

MIX_Track *AudioManager::spawnTrack = nullptr;
MIX_Track *AudioManager::sfxTrack = nullptr;
std::unordered_map<std::string, MIX_Audio *> AudioManager::audio;
std::string AudioManager::currentSpawnAudioLoop{};


AudioManager::AudioManager() {
    if (!MIX_Init()) {
        std::cout << "MIX_Init() failed." << std::endl;
        return;
    }

    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if (!mixer) {
        std::cout << "MIX_CreateMixerDevice() failed." << std::endl;
        return;
    }

    musicTrack = MIX_CreateTrack(mixer);
    sfxTrack = MIX_CreateTrack(mixer);
    spawnTrack = MIX_CreateTrack(mixer);
    MIX_SetTrackGain(musicTrack, 0.1f);
}

void AudioManager::loadAudio(const std::string &name, const char *path) const {
    if (audio.contains(name)) {
        return;
    }
    auto audioPtr = MIX_LoadAudio(mixer, path, true);
    if (!audioPtr) {
        std::cout << "MIX_LoadAudio() failed." << std::endl;
        return;
    }
    audio.emplace(name, audioPtr);
}

void AudioManager::playMusic(const std::string &name) const {
    if (!MIX_SetTrackAudio(musicTrack, audio[name])) {
        std::cout << "MIX_SetTrackAudio() failed." << std::endl;
        return;
    }
    //loop music due to -1
    MIX_PlayTrack(musicTrack, -1);

    std::cout << "Playing music: " << name << std::endl;
}

void AudioManager::stopMusic() const {
    MIX_StopTrack(musicTrack, 0);
}

void AudioManager::playSfx(const std::string &name) {
    if (!MIX_SetTrackAudio(sfxTrack, audio[name])) {
        std::cout << "MIX_SetTrackAudio() failed." << std::endl;
        return;
    }

    MIX_PlayTrack(sfxTrack, 0);
}

void AudioManager::playSpawn(const std::string &name) {
    if (!audio.contains(name)) {
        std::cout << "Cant find spawn music: " << name << std::endl;
        return;
    }
    if (currentSpawnAudioLoop == name) {
        return;
    }
    MIX_StopTrack(spawnTrack, 0);

    if (!MIX_SetTrackAudio(spawnTrack, audio[name])) {
        std::cout << "MIX_SetTrackAudio() failed for: " << name << std::endl;
        return;
    }

    MIX_PlayTrack(spawnTrack, -1);
    currentSpawnAudioLoop = name;
}

void AudioManager::stopSpawn() {
    if (currentSpawnAudioLoop.empty()) return;

    MIX_StopTrack(spawnTrack, 0);
    currentSpawnAudioLoop.clear();
}
