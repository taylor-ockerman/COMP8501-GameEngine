//
// Created by taylo on 2/25/2026.
//

#ifndef INC_8051TUTORIAL_SCENE_H
#define INC_8051TUTORIAL_SCENE_H
#include <SDL3/SDL_events.h>

#include "ParticleGrid.h"
#include "World.h"
#include "SceneType.h"

class Scene {
public:
    Scene(SceneType sceneType, const char *sceneName, const char *mapPath, int windowWidth, int windowHeight);

    ~Scene();

    void update(const float dt, const SDL_Event &e) {
        world.update(dt, e, sceneType, grid);
    }

    void render(SDL_Renderer *renderer) {
        world.render(renderer);
        if (grid) {
            float camOffx = 0.0f;
            float camOffy = 0.0f;

            for (auto &e: world.getEntities()) {
                if (e->hasComponent<Camera>()) {
                    auto &camera = e->getComponent<Camera>();
                    camOffx = camera.view.x;
                    camOffy = camera.view.y;
                    break;
                }
            }
        }
    }

    World world;

    const std::string &getName() const {
        return name;
    }

    void setUpParticleGrid(int windowWidth, int windowHeight, int cellSize) {
        grid = new ParticleGrid(windowWidth, windowHeight, cellSize);
    };

    void resetScene();

private:
    std::string name;
    SceneType sceneType;
    ParticleGrid *grid;

    void createProjectile(Vector2D pos, Vector2D dir, int speed);

    void initMainMenu(int windowWidth, int windowHeight);

    void initGameplay(const char *mappath, int windowWidth, int windowHeight);

    Entity &createSettingsOverlay(int windowWidth, int windowHeight);

    Entity &createCogButton(int windowWidth, int windowHeight, Entity &overlay);

    void createSettingsUIComponents(Entity &overlay);

    void toggleSettingsOverlayVisibility(Entity &overlay);

    Entity &createPlayerPositionLabel();
};
#endif //INC_8051TUTORIAL_SCENE_H
