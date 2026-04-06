//
// Created by taylo on 2/25/2026.
//

#include "Scene.h"
#include "Game.h"
#include "AssetManager.h"

Scene::Scene(SceneType sceneType, const char *sceneName, const char *mapPath, const int windowWidth,
             const int windowHeight) : name(
                                           sceneName), sceneType(sceneType), grid(nullptr) {
    if (sceneType == SceneType::MainMenu) {
        initMainMenu(windowWidth, windowHeight);
        return;
    }
    //grid = nullptr;
    initGameplay(mapPath, windowWidth, windowHeight);
}

Scene::~Scene() {
    if (grid != nullptr) {
        delete grid;
        grid = nullptr;
    }
}

//not used right now
//keeping this incase i want to reload a level later on
void Scene::resetScene() {
    world.destroyAllParticles(grid);

    if (grid != nullptr) {
        delete grid;
        grid = nullptr;
    }
    world.clearAllEntities();
}

void Scene::createProjectile(Vector2D pos, Vector2D dir, int speed) {
}

void Scene::initMainMenu(int windowWidth, int windowHeight) {
    //camera
    auto &cam = world.createEntity();
    cam.addComponent<Camera>(SDL_FRect{}, 0.0f, 0.0f, 1.0f);

    //menu
    auto &menu(world.createEntity());
    auto menuTransform = menu.addComponent<Transform>(Vector2D(0, 0), 0.0f, 1.0f);

    SDL_Texture *tex = TextureManager::load("../assets/menu.png");
    SDL_FRect menuSrc{0, 0, 2752.0f, 1536.0f}; //dimensions are so it fits the png i have for the mainmenu

    SDL_FRect menuDest{menuTransform.position.x, menuTransform.position.y, (float) windowWidth, (float) windowHeight};
    menu.addComponent<Sprite>(tex, menuSrc, menuDest);

    auto &settingsOverlay = createSettingsOverlay(windowWidth, windowHeight);
    createCogButton(windowWidth, windowHeight, settingsOverlay);
}

void Scene::initGameplay(const char *mapPath, int windowWidth, int windowHeight) {
    //load out map
    world.getMap().load(mapPath, TextureManager::load("../assets/grassland_bg_tileset.png"),
                        TextureManager::load("../assets/grassland_tileset.png"));
    //remove old grid before creating new one
    delete grid;
    grid = nullptr;
    grid = new ParticleGrid(world.getMap().width * world.getMap().getTileSize(),
                            world.getMap().height * world.getMap().getTileSize(), 4);

    //setUpParticleGrid(windowWidth, windowHeight, 4);
    for (auto &collider: world.getMap().colliders) {
        auto &e = world.createEntity();
        e.addComponent<Transform>(Vector2D(collider.rect.x, collider.rect.y), 0.0f, 1.0f);
        auto &c = e.addComponent<Collider>("wall"); //change to wall to turn on red colliders again
        c.rect.x = collider.rect.x;
        c.rect.y = collider.rect.y;
        c.rect.w = collider.rect.w;
        c.rect.h = collider.rect.h;
        //adding wall colliders to grid system.
        grid->addWallRect(c.rect);
        //uncomment this block to show red boxes on wall colliders
        // SDL_Texture *tex = TextureManager::load("../assets/tileset.png");
        // SDL_FRect colSrc{0, 32, 32, 32};
        // SDL_FRect colDst{c.rect.x, c.rect.y, c.rect.w, c.rect.h};
        // e.addComponent<Sprite>(tex, colSrc, colDst);
    }

    //dont need coins
    // for (auto &sp: world.getMap().coins) {
    //     auto &item = world.createEntity();
    //     item.addComponent<Transform>(Vector2D(sp.rect.x, sp.rect.y), 0.0f, 1.0f);
    //     auto &c = item.addComponent<Collider>("item");
    //     c.rect.x = sp.rect.x;
    //     c.rect.y = sp.rect.y;
    //     SDL_Texture *itemTex = TextureManager::load("../assets/coin.png");
    //     // SDL_Texture *itemTex = TextureManager::load("../assets/tileset.png"); //uncomment for red box on collider
    //     SDL_FRect itemSrc{0, 0, 32, 32};
    //     SDL_FRect itemDst{c.rect.x, c.rect.y, 32, 32};
    //     auto &s = item.addComponent<Sprite>(itemTex, itemSrc, itemDst);
    //     c.rect.w = s.dst.w;
    //     c.rect.h = s.dst.h;
    // }

    auto &cam = world.createEntity();
    SDL_FRect camView{};
    camView.w = windowWidth;
    camView.h = windowHeight;
    cam.addComponent<Camera>(camView, (float) (world.getMap().width * world.getMap().tileWidth),
                             (float) (world.getMap().height * world.getMap().tileHeight));

    //add entities
    auto &player(world.createEntity());
    auto &playerTrans = player.addComponent<Transform>(Vector2D(5.0f, 5.0f), 0.0f, 1.0f);
    player.addComponent<Velocity>(Vector2D(0.0f, 0.0f), 240.0f);
    player.addComponent<Acceleration>(Vector2D(0.0f, 0.0f), 0.0f, false);
    //comment this block out to remove player sprite and enable red texture on collider to test collider box
    Animation anim = AssetManager::getAnimation("player");
    player.addComponent<Animation>(anim);
    SDL_Texture *texture = TextureManager::load("../assets/animations/human_anim.png");
    SDL_FRect playerSrc = anim.clips[anim.currentClip].frameIndices[0];

    SDL_FRect playerDest{playerTrans.position.x, playerTrans.position.y, 64, 64};
    player.addComponent<Sprite>(texture, playerSrc, playerDest);


    auto &playerCollider = player.addComponent<Collider>("player");
    playerCollider.rect.w = 10; //hard coded values for player collider rect size
    playerCollider.rect.h = 25;
    playerCollider.offset.x = (playerDest.w - playerCollider.rect.w) / 2;
    playerCollider.offset.y = (playerDest.h - playerCollider.rect.h) / 2;
    player.addComponent<PlayerTag>();

    //uncomment to add red texture to player collision box
    // SDL_Texture *red = TextureManager::load("../assets/tileset.png");
    // SDL_FRect redSrc{0, 32, 32, 32};
    // player.addComponent<Sprite>(red, redSrc,
    //                             SDL_FRect(playerTrans.position.x + playerCollider.offset.x,
    //                                       playerTrans.position.y + playerCollider.offset.y, playerCollider.rect.w,
    //                                       playerCollider.rect.h));

    player.addComponent<Health>(Game::gameState.playerHealth);

    //dont need crows spawning
    // auto &spawner(world.createEntity());
    // Transform t = spawner.addComponent<Transform>(Vector2D(windowWidth / 2, windowHeight - 5), 0.0f, 1.0f);
    // spawner.addComponent<TimedSpawner>(2.0f, [this, t] {
    //     //create our projectile
    //     auto &e(world.createDeferredEntity());
    //     e.addComponent<Transform>(Vector2D(t.position.x, t.position.y), 0.0f, 1.0f);
    //     e.addComponent<Velocity>(Vector2D(0, -1), 200.0f);
    //     //e.addComponent<Acceleration>();
    //
    //     auto &anim = AssetManager::getAnimation("enemy");
    //     e.addComponent<Animation>(anim);
    //
    //     SDL_Texture *tex = TextureManager::load("../assets/animations/bird_anim.png");
    //     SDL_FRect src = {0, 0, 32, 32};
    //     SDL_FRect dest{t.position.x, t.position.y, 32, 32};
    //     e.addComponent<Sprite>(tex, src, dest);
    //
    //     auto &c = e.addComponent<Collider>("projectile");
    //     c.rect.w = dest.w;
    //     c.rect.h = dest.h;
    //
    //     e.addComponent<ProjectileTag>();
    // });

    //add scene state
    auto &state(world.createEntity());
    state.addComponent<SceneState>();

    //createPlayerPositionLabel();
    createParticleCountLabel();
    createMenuOverlay(windowWidth, windowHeight, cam.getComponent<Camera>());
    createSpawnerHUD(windowHeight);
    //create brush state that is used to spawn particles on the mouse location
    auto &brushState = world.createEntity();
    brushState.addComponent<BrushState>();
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

Entity &Scene::createMenuOverlay(int windowWidth, int windowHeight, Camera &cam) {
    auto &overlay(world.createEntity());
    SDL_Texture *overlayTex = TextureManager::load("../assets/ui/settings.jpg");
    SDL_FRect overlaySrc{0, 0, (windowWidth * 0.60f), (windowHeight * 0.70f)};
    SDL_FRect overlayDest{
        (float) windowWidth / 2 - overlaySrc.w / 2, (float) windowHeight / 2 - overlaySrc.h / 2, overlaySrc.w,
        overlaySrc.h
    };
    overlay.addComponent<Transform>(Vector2D(overlayDest.x, overlayDest.y), 0.0f, 1.0f);
    overlay.addComponent<Sprite>(overlayTex, overlaySrc, overlayDest, RenderLayer::UI, false);
    overlay.addComponent<MenuTag>();
    createMenuUIComponents(overlay);
    return overlay;
}

void Scene::createMenuUIComponents(Entity &overlay) {
    if (!overlay.hasComponent<Children>()) {
        overlay.addComponent<Children>();
    }
    auto &overlayTransform = overlay.getComponent<Transform>();
    auto &overlaySprite = overlay.getComponent<Sprite>();

    auto &controlText = createMenuControlText();
    auto &t = controlText.getComponent<Transform>();
    t.position.y = overlayTransform.position.y + overlaySprite.dst.h / 8;
    t.position.x = overlayTransform.position.x + overlaySprite.dst.w / 8;

    auto &parentChildren = overlay.getComponent<Children>();
    parentChildren.children.push_back(&controlText);
}


Entity &Scene::createPlayerPositionLabel() {
    auto &playerPositionLabel(world.createEntity());
    Label label = {
        "Test string",
        AssetManager::getFont("pixelLarge"),
        {255, 255, 255, 255},
        LabelType::PlayerPosition,
        "playerPos"
    };
    TextureManager::loadLabel(label);
    playerPositionLabel.addComponent<Label>(label);
    playerPositionLabel.addComponent<Transform>(Vector2D{10.0f, 50.0f}, 0.0f, 1.0f);
    return playerPositionLabel;
}

Entity &Scene::createParticleCountLabel() {
    auto &playerPositionLabel(world.createEntity());
    Label label = {
        "Test string",
        AssetManager::getFont("pixelLarge"),
        {255, 255, 255, 255},
        LabelType::ParticleCount,
        "particleCount"
    };
    TextureManager::loadLabel(label);
    playerPositionLabel.addComponent<Label>(label);
    playerPositionLabel.addComponent<Transform>(Vector2D{10.0f, 10.0f}, 0.0f, 1.0f);
    return playerPositionLabel;
}

Entity &Scene::createMenuControlText() {
    auto &text(world.createEntity());
    Label label = {
        "CONTROLS\n"
        "WASD: Move character\n"
        "Space: Jump\n"
        "Left Click: Spawn particle\n"
        "1-8: Select particle to spawn\n"
        "Scroll Wheel: Change Spawner Size\n"
        "Esc: Toggle menu\n"
        "F10: Quit game",
        AssetManager::getFont("pixelLarge"),
        {0, 0, 0, 0},
        LabelType::MenuText,
        "menuText",
    };
    label.dirty = true;
    label.visible = false;
    TextureManager::loadLabel(label);
    text.addComponent<Label>(label);
    text.addComponent<Transform>(Vector2D{1.0f, 1.0f}, 0.0f, 1.0f);
    return text;
}

Entity &Scene::createSpawnerLabel() {
    auto &text(world.createEntity());
    Label label = {
        "Sand",
        AssetManager::getFont("pixelLarge"),
        {255, 255, 255, 255},
        LabelType::SpawnerText,
        "spawnerText",
    };
    label.dirty = true;
    TextureManager::loadLabel(label);
    text.addComponent<Label>(label);
    text.addComponent<Transform>(Vector2D{1.0f, 1.0f}, 0.0f, 1.0f);
    return text;
}

Entity &Scene::createSpawnerHUD(int windowHeight) {
    Entity &particleSpawnerHUD = world.createEntity();
    Transform t = particleSpawnerHUD.addComponent<Transform>(Vector2D(6.0f, windowHeight - 70.0f),
                                                             0.0f,
                                                             1.0f);
    SDL_Texture *tex = TextureManager::load("../assets/particle_tileset.png");
    SDL_FRect src{0, 0, 128, 128};
    SDL_FRect dst{t.position.x, t.position.y, 64, 64};
    particleSpawnerHUD.addComponent<Sprite>(tex, src, dst, RenderLayer::UI, true);
    particleSpawnerHUD.addComponent<SpawnerHUDTag>();
    auto &spawnLabel = createSpawnerLabel();
    auto &spawnT = spawnLabel.getComponent<Transform>();
    auto &spawnHUDT = particleSpawnerHUD.getComponent<Transform>();
    spawnLabel.addComponent<Parent>(&particleSpawnerHUD);
    particleSpawnerHUD.addComponent<Children>().children.push_back(&spawnLabel);
    spawnT.position.x = spawnHUDT.position.x;
    spawnT.position.y = spawnHUDT.position.y - 40;
    return particleSpawnerHUD;
}


