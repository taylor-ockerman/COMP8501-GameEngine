cmake_minimum_required(VERSION 4.0)
project(8051Tutorial)

set(CMAKE_CXX_STANDARD 20)

#Set SDL3 include and lib paths
if (WIN32)
    set(SDL3_PATH "C:/Dev/SDL3")
    set(SDL3_IMAGE_PATH "C:/Dev/SDL3_image")
    include_directories(
            ${SDL3_PATH}/include
            ${SDL3_IMAGE_PATH}/include
            ${PROJET_SOURCE_DIR}/src
            ${PROJET_SOURCE_DIR}/src/utils
            ${PROJET_SOURCE_DIR}/src/ecs
            ${PROJET_SOURCE_DIR}/src/ecs/system
            ${PROJET_SOURCE_DIR}/src/vendor
    )
    link_directories(
            ${SDL3_PATH}/lib
            ${SDL3_IMAGE_PATH}/lib
    )
endif ()

add_executable(COMP8051_Tutorial src/main.cpp
        src/Game.cpp
        src/TextureManager.cpp
        src/Map.cpp
        src/GameObject.cpp
        src/utils/Vector2D.cpp
        src/utils/Collision.cpp
        src/ecs/system/CollisionSystem.cpp
        src/ecs/World.cpp
        src/vendor/tinyxml2.cpp
        src/manager/AssetManager.cpp
        src/manager/Scene.cpp
        src/manager/SceneManager.h)

target_link_libraries(COMP8051_Tutorial
        SDL3
        SDL3_image
)

