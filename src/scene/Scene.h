//
// Created by taylo on 2/25/2026.
//

#ifndef INC_8051TUTORIAL_SCENE_H
#define INC_8051TUTORIAL_SCENE_H
#include <SDL3/SDL_events.h>

#include "../ecs/World.h"
#include "SceneType.h"

class Scene {
public:
    Scene(SceneType sceneType, const char *sceneName, const char *mapPath, int windowWidth, int windowHeight);

    void update(const float dt, const SDL_Event &e) {
        world.update(dt, e, sceneType);
    }

    void render() {
        world.render();
    }

    World world;

    const std::string &getName() const {
        return name;
    }

private:
    std::string name;
    SceneType sceneType;

    void createProjectile(Vector2D pos, Vector2D dir, int speed);

    void initMainMenu(int windowWidth, int windowHeight);

    void initGameplay(const char *mappath, int windowWidth, int windowHeight);

    Entity &createSettingsOverlay(int windowWidth, int windowHeight);

    Entity &createCogButton(int windowWidth, int windowHeight, Entity &overlay);

    void createSettingsUIComponents(Entity &overlay);

    void toggleSettingsOverlayVisibility(Entity &overlay);

    Entity& createPlayerPositionLabel();
};
#endif //INC_8051TUTORIAL_SCENE_H
