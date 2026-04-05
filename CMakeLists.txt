cmake_minimum_required(VERSION 4.0)
project(8051Tutorial)

set(CMAKE_CXX_STANDARD 20)

#Set SDL3 include and lib paths
if (WIN32)
    set(SDL3_PATH "C:/Dev/SDL3_3_4_2")
    set(SDL3_IMAGE_PATH "C:/Dev/SDL3_image")
    set(SDL3_TTF_PATH "C:/Dev/SDL3_ttf")
    set(SDL3_MIXER_PATH "C:/Dev/SDL3_mixer")
    include_directories(
            ${SDL3_PATH}/include
            ${SDL3_IMAGE_PATH}/include
            ${SDL3_TTF_PATH}/include
            ${SDL3_MIXER_PATH}/include
            ${PROJECT_SOURCE_DIR}/src
            ${PROJECT_SOURCE_DIR}/src/utils
            ${PROJECT_SOURCE_DIR}/src/ecs
            ${PROJECT_SOURCE_DIR}/src/ecs/system
            ${PROJECT_SOURCE_DIR}/src/ecs/event
            ${PROJECT_SOURCE_DIR}/src/vendor
            ${PROJECT_SOURCE_DIR}/src/manager
            ${PROJECT_SOURCE_DIR}/src/scene
    )
    link_directories(
            ${SDL3_PATH}/lib
            ${SDL3_IMAGE_PATH}/lib
            ${SDL3_TTF_PATH}/lib
            ${SDL3_MIXER_PATH}/lib
    )
endif ()

add_executable(COMP8051_Tutorial src/main.cpp
        src/Game.cpp
        src/manager/TextureManager.cpp
        src/Map.cpp
        src/GameObject.cpp
        src/utils/Vector2D.cpp
        src/utils/Collision.cpp
        src/ecs/system/CollisionSystem.cpp
        src/vendor/tinyxml2.cpp
        src/manager/AssetManager.cpp
        src/scene/Scene.cpp
        src/ecs/system/EventResponseSystem.cpp
        src/ecs/system/MainMenuSystem.cpp
        src/ecs/system/MouseInputSystem.cpp
        src/ecs/system/ColliderSyncSystem.cpp
        src/utils/CollisionResolution.cpp
        src/ecs/system/KeyboardInputSystem.cpp
        src/utils/ParticleHelpers.h
        src/ecs/system/ParticleSyncSystem.cpp
        src/utils/ParticlePlacementHelper.cpp
        src/ecs/system/ParticleInteractionSystem.h
        src/manager/AudioManager.cpp
        src/ecs/system/HUDSystem.cpp
        src/ecs/system/UIRenderSystem.cpp
        src/ecs/system/ParticleSpawnDeSpawnSystem.cpp)

target_link_libraries(COMP8051_Tutorial
        SDL3
        SDL3_image
        SDL3_ttf
        SDL3_mixer
)

