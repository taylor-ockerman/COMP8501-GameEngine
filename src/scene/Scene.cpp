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
        initMainMenu(windowWidth, windowHeight);
        return;
    }
    initGameplay(mapPath, windowWidth, windowHeight);
}

void Scene::createProjectile(Vector2D pos, Vector2D dir, int speed) {
}

void Scene::initMainMenu(int windowWidth, int windowHeight) {
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

    auto &settingsOverlay = createSettingsOverlay(windowWidth, windowHeight);
    createCogButton(windowWidth, windowHeight, settingsOverlay);
}

void Scene::initGameplay(const char *mapPath, int windowWidth, int windowHeight) {
    //load out map
    world.getMap().load(mapPath, TextureManager::load("../assets/tileset2.png"));
    for (auto &collider: world.getMap().colliders) {
        auto &e = world.createEntity();
        e.addComponent<Transform>(Vector2D(collider.rect.x, collider.rect.y), 0.0f, 1.0f);
        auto &c = e.addComponent<Collider>("wall"); //change to wall to turn on red colliders again
        c.rect.x = collider.rect.x;
        c.rect.y = collider.rect.y;
        c.rect.w = collider.rect.w;
        c.rect.h = collider.rect.h;
        //uncomment this block to show red boxes on wall colliders
        // SDL_Texture *tex = TextureManager::load("../assets/tileset.png");
        // SDL_FRect colSrc{0, 32, 32, 32};
        // SDL_FRect colDst{c.rect.x, c.rect.y, c.rect.w, c.rect.h};
        // e.addComponent<Sprite>(tex, colSrc, colDst);
    }

    for (auto &sp: world.getMap().coins) {
        auto &item = world.createEntity();
        item.addComponent<Transform>(Vector2D(sp.rect.x, sp.rect.y), 0.0f, 1.0f);
        auto &c = item.addComponent<Collider>("item");
        c.rect.x = sp.rect.x;
        c.rect.y = sp.rect.y;

        SDL_Texture *itemTex = TextureManager::load("../assets/coin.png");
        SDL_FRect itemSrc{0, 0, 32, 32};
        SDL_FRect itemDst{c.rect.x, c.rect.y, 32, 32};
        item.addComponent<Sprite>(itemTex, itemSrc, itemDst);
    }

    auto &cam = world.createEntity();
    SDL_FRect camView{};
    camView.w = windowWidth;
    camView.h = windowHeight;
    cam.addComponent<Camera>(camView, world.getMap().width * 32.0f, world.getMap().height * 32.0f);

    //add entities
    auto &player(world.createEntity());
    auto &playerTrans = player.addComponent<Transform>(Vector2D(5.0f, 5.0f), 0.0f, 1.0f);
    player.addComponent<Velocity>(Vector2D(0.0f, 0.0f), 240.0f);
    player.addComponent<Acceleration>(Vector2D(0.0f, 0.0f), 0.0f, false);
    Animation anim = AssetManager::getAnimation("player");
    player.addComponent<Animation>(anim);

    SDL_Texture *texture = TextureManager::load("../assets/animations/human_anim.png");
    SDL_FRect playerSrc = anim.clips[anim.currentClip].frameIndices[0];
    SDL_FRect playerDest{playerTrans.position.x, playerTrans.position.y, 64, 64};
    player.addComponent<Sprite>(texture, playerSrc, playerDest);


    auto &playerCollider = player.addComponent<Collider>("player");
    playerCollider.rect.w = 24; //hard coded values for player collider rect size
    playerCollider.rect.h = 34;
    playerCollider.offset.x = (playerDest.w - playerCollider.rect.w) / 2;
    playerCollider.offset.y = (playerDest.h - playerCollider.rect.h) / 2;

    player.addComponent<PlayerTag>();

    player.addComponent<Health>(Game::gameState.playerHealth);

    auto &spawner(world.createEntity());
    Transform t = spawner.addComponent<Transform>(Vector2D(windowWidth / 2, windowHeight - 5), 0.0f, 1.0f);
    spawner.addComponent<TimedSpawner>(2.0f, [this, t] {
        //create our projectile
        auto &e(world.createDeferredEntity());
        e.addComponent<Transform>(Vector2D(t.position.x, t.position.y), 0.0f, 1.0f);
        e.addComponent<Velocity>(Vector2D(0, -1), 200.0f);
        //e.addComponent<Acceleration>();

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

    //add scene state
    auto &state(world.createEntity());
    state.addComponent<SceneState>();
}

Entity &Scene::createSettingsOverlay(int windowWidth, int windowHeight) {
    auto &overlay(world.createEntity());
    SDL_Texture *overlayTex = TextureManager::load("../assets/ui/settings.jpg");
    SDL_FRect overlaySrc{0, 0, windowWidth * 0.85f, windowHeight * 0.85f};
    SDL_FRect overlayDest{
        (float) windowWidth / 2 - overlaySrc.w / 2, (float) windowHeight / 2 - overlaySrc.h / 2, overlaySrc.w,
        overlaySrc.h
    };
    overlay.addComponent<Transform>(Vector2D(overlayDest.x, overlayDest.y), 0.0f, 1.0f);
    overlay.addComponent<Sprite>(overlayTex, overlaySrc, overlayDest, RenderLayer::UI, false);
    createSettingsUIComponents(overlay);
    return overlay;
}

Entity &Scene::createCogButton(int windowWidth, int windowHeight, Entity &overlay) {
    //std::cout << "Creating Cog Button" << std::endl;
    auto &cog(world.createEntity());
    auto &cogTransform = cog.addComponent<Transform>(Vector2D((float) windowWidth - 50, (float) windowHeight - 50),
                                                     0.0f, 1.0f);
    SDL_Texture *tex = TextureManager::load("../assets/ui/cog.png");
    SDL_FRect cogSrc{0, 0, 32, 32};
    SDL_FRect cogDest{cogTransform.position.x, cogTransform.position.y, cogSrc.w, cogSrc.h};
    cog.addComponent<Sprite>(tex, cogSrc, cogDest, RenderLayer::UI, true);
    cog.addComponent<Collider>("ui", cogDest);
    auto &clickable = cog.addComponent<Clickable>();
    clickable.onPressed = [&cogTransform] {
        cogTransform.scale = 0.5f;
    };
    clickable.onReleased = [this,&cogTransform, &overlay] {
        cogTransform.scale = 1.0f;
        toggleSettingsOverlayVisibility(overlay);
    };
    clickable.onCancel = [&cogTransform] {
        cogTransform.scale = 1.0f;
    };
    return cog;
}

void Scene::createSettingsUIComponents(Entity &overlay) {
    if (!overlay.hasComponent<Children>()) {
        overlay.addComponent<Children>();
    }
    auto &overlayTransform = overlay.getComponent<Transform>();
    auto &overlaySprite = overlay.getComponent<Sprite>();

    float baseX = overlayTransform.position.x;
    float baseY = overlayTransform.position.y;

    auto &closeButton = world.createEntity();
    auto &closeTransform = closeButton.addComponent<Transform>(Vector2D(baseX + overlaySprite.dst.w - 40, baseY + 10),
                                                               0.0f, 1.0f);
    SDL_Texture *text = TextureManager::load("../assets/ui/close.png");
    SDL_FRect closeSrc{0, 0, (float) 32, (float) 32};
    SDL_FRect closeDest{closeTransform.position.x, closeTransform.position.y, closeSrc.w, closeSrc.h};
    closeButton.addComponent<Sprite>(text, closeSrc, closeDest, RenderLayer::UI, false);
    closeButton.addComponent<Collider>("ui", closeDest);

    auto &clickable = closeButton.addComponent<Clickable>();
    clickable.onPressed = [&closeTransform] {
        closeTransform.scale = 0.5f;
    };
    clickable.onReleased = [this,&overlay,&closeTransform] {
        closeTransform.scale = 1.0f;
        toggleSettingsOverlayVisibility(overlay);
    };
    clickable.onCancel = [&closeTransform] {
        closeTransform.scale = 1.0f;
    };

    closeButton.addComponent<Parent>(&overlay);
    auto &parentChildren = overlay.getComponent<Children>();
    parentChildren.children.push_back(&closeButton);
}

void Scene::toggleSettingsOverlayVisibility(Entity &overlay) {
    auto &sprite = overlay.getComponent<Sprite>();
    bool newVisibility = !sprite.visible;
    sprite.visible = newVisibility;
    if (overlay.hasComponent<Children>()) {
        auto &c = overlay.getComponent<Children>();

        for (auto &child: c.children) {
            if (child && child->hasComponent<Sprite>()) {
                child->getComponent<Sprite>().visible = newVisibility;
            }
            if (child && child->hasComponent<Collider>()) {
                child->getComponent<Collider>().enabled = newVisibility;
            }
        }
    }
}
