//
// Created by taylo on 2/25/2026.
//

#include "Scene.h"
#include "../Game.h"
#include "../manager/AssetManager.h"

Scene::Scene(SceneType sceneType, const char *sceneName, const char *mapPath, const int windowWidth,
             const int windowHeight) : name(
                                           sceneName), sceneType(sceneType) {
    if (sceneType == SceneType::MainMenu) {
        //camera
        auto &cam = world.createEntity();
        cam.addComponent<Camera>();

        //menu
        auto &menu(world.createEntity());
        auto menuTransform = menu.addComponent<Transform>(Vector2D(0, 0), 0.0f, 1.0f);

        SDL_Texture *tex = TextureManager::load("../assets/menu.png");
        SDL_FRect menuSrc{0, 0, (float) windowWidth, (float) windowHeight};
        SDL_FRect menuDest{menuTransform.position.x, menuTransform.position.y, menuSrc.w, menuSrc.h};
        menu.addComponent<Sprite>(tex, menuSrc, menuDest);
        return;
    }
    //load out map
    world.getMap().load(mapPath, TextureManager::load("../assets/tileset.png"));
    for (auto &collider: world.getMap().colliders) {
        auto &e = world.createEntity();
        e.addComponent<Transform>(Vector2D(collider.rect.x, collider.rect.y), 0.0f, 1.0f);
        auto &c = e.addComponent<Collider>("no-wall"); //change to wall to turn on red colliders again
        c.rect.x = collider.rect.x;
        c.rect.y = collider.rect.y;
        c.rect.w = collider.rect.w;
        c.rect.h = collider.rect.h;

        SDL_Texture *tex = TextureManager::load("../assets/tileset.png");
        SDL_FRect colSrc{0, 32, 32, 32};
        SDL_FRect colDst{c.rect.x, c.rect.y, c.rect.w, c.rect.h};
        e.addComponent<Sprite>(tex, colSrc, colDst);
    }
    //add entities
    auto &item(world.createEntity());
    auto &itemTransform = item.addComponent<Transform>(Vector2D(100.0f, 200.0f), 0.0f, 1.0f);
    for (auto &collider: world.getMap().coins) {
        auto &e = world.createEntity();
        e.addComponent<Transform>(Vector2D(collider.rect.x, collider.rect.y), 0.0f, 1.0f);
        auto &c = e.addComponent<Collider>("item");
        c.rect.x = collider.rect.x;
        c.rect.y = collider.rect.y;
        // c.rect.w = collider.rect.w;
        // c.rect.h = collider.rect.h;

        SDL_Texture *tex = TextureManager::load("../assets/coin.png");
        SDL_FRect colSrc{0, 0, 32, 32};
        SDL_FRect colDst{c.rect.x, c.rect.y, 32, 32};
        e.addComponent<Sprite>(tex, colSrc, colDst);
    }
    SDL_FRect itemSrc{0, 0, 32, 32};

    auto &cam = world.createEntity();
    SDL_FRect camView{};
    camView.w = windowWidth;
    camView.h = windowHeight;
    cam.addComponent<Camera>(camView, world.getMap().width * 32.0f, world.getMap().height * 32.0f);

    //add entities
    auto &player(world.createEntity());
    auto &playerTrans = player.addComponent<Transform>(Vector2D(5.0f, 5.0f), 0.0f, 1.0f);
    player.addComponent<Velocity>(Vector2D(0.0f, 0.0f), 240.0f);
    Animation anim = AssetManager::getAnimation("player");
    player.addComponent<Animation>(anim);

    SDL_Texture *texture = TextureManager::load("../assets/animations/human_anim.png");
    SDL_FRect playerSrc = anim.clips[anim.currentClip].frameIndices[0];
    SDL_FRect playerDest{playerTrans.position.x, playerTrans.position.y, 128, 128};

    player.addComponent<Sprite>(texture, playerSrc, playerDest);

    auto &playerCollider = player.addComponent<Collider>("player");
    playerCollider.rect.w = playerDest.w;
    playerCollider.rect.h = playerDest.h;

    player.addComponent<PlayerTag>();

    player.addComponent<Health>(Game::gameState.playerHealth);

    auto &spawner(world.createEntity());
    Transform t = spawner.addComponent<Transform>(Vector2D(windowWidth / 2, windowHeight - 5), 0.0f, 1.0f);
    spawner.addComponent<TimedSpawner>(2.0f, [this, t] {
        //create our projectile
        auto &e(world.createDeferredEntity());
        e.addComponent<Transform>(Vector2D(t.position.x, t.position.y), 0.0f, 1.0f);
        e.addComponent<Velocity>(Vector2D(0, -1), 100.0f);

        auto &anim = AssetManager::getAnimation("enemy");
        e.addComponent<Animation>(anim);

        SDL_Texture *tex = TextureManager::load("../assets/animations/bird_anim.png");
        SDL_FRect src = {0, 0, 32, 32};
        SDL_FRect dest{t.position.x, t.position.y, 32, 32};
        e.addComponent<Sprite>(tex, src, dest);
        auto &c = e.addComponent<Collider>("projectile");
        c.rect.w = dest.w;
        c.rect.h = dest.h;

        e.addComponent<ProjectileTag>();
    });

    auto &state(world.createEntity());
    state.addComponent<SceneState>();
}

void Scene::createProjectile(Vector2D pos, Vector2D dir, int speed) {
}
