//
// Created by taylo on 2/25/2026.
//

#ifndef INC_8051TUTORIAL_SCENEMANAGER_H
#define INC_8051TUTORIAL_SCENEMANAGER_H
#include <memory>
#include <string>
#include <unordered_map>

#include "../scene/Scene.h"

struct SceneParams {
    SceneType sceneType;
    const char *name;
    const char *mapPath;
    int windowWidth;
    int windowHeight;
};

class SceneManager {
    std::unordered_map<std::string, SceneParams> scenes;
    std::string pendingScene;

    void changeScene(const std::string &name) {
        auto it = scenes.find(name);
        if (it != scenes.end()) {
            const auto &params = it->second;
            //create Scene object
            //build scene
            currentScene = std::make_unique<Scene>(
                params.sceneType,
                params.name,
                params.mapPath,
                params.windowWidth,
                params.windowHeight
            );
        } else {
            std::cerr << "Scene " << name << " not found!" << std::endl;
        }
    }

public:
    std::unique_ptr<Scene> currentScene;

    void loadScene(SceneType sceneType, const char *sceneName, const char *mapPath, int windowWidth, int windowHeight) {
        scenes[sceneName] = {sceneType, sceneName, mapPath, windowWidth, windowHeight};
    }

    void changeSceneDeferred(const std::string &name) {
        pendingScene = name;
    }

    void update(const float dt, const SDL_Event &e) {
        if (currentScene) currentScene->update(dt, e); //all our world updates
        if (!pendingScene.empty()) {
            changeScene(pendingScene);
            pendingScene.clear();
        }
    }

    void render() const {
        if (currentScene) currentScene->render();
    }
};
#endif //INC_8051TUTORIAL_SCENEMANAGER_H
