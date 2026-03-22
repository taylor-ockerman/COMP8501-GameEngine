//
// Created by taylo on 3/22/2026.
//
#include "CollisionResolution.h"

#include <SDL3/SDL_rect.h>

#include "Component.h"
#include "Entity.h"

void CollisionResolution::resolvePlayerWall(Entity &player, Entity &wall) {
    if (!player.hasComponent<Transform>() || !player.hasComponent<Collider>() ||
        !player.hasComponent<Velocity>() || !player.hasComponent<Acceleration>()) {
        return;
    }

    if (!wall.hasComponent<Collider>()) {
        return;
    }

    auto &t = player.getComponent<Transform>();
    auto &cPlayer = player.getComponent<Collider>();
    auto &cWall = wall.getComponent<Collider>();
    auto &v = player.getComponent<Velocity>();
    auto &a = player.getComponent<Acceleration>();

    SDL_FRect &p = cPlayer.rect;
    SDL_FRect &w = cWall.rect;

    float overlapLeft = (p.x + p.w) - w.x;
    float overlapRight = (w.x + w.w) - p.x;
    float overlapTop = (p.y + p.h) - w.y;
    float overlapBottom = (w.y + w.h) - p.y;

    float minX = std::min(overlapLeft, overlapRight);
    float minY = std::min(overlapTop, overlapBottom);

    if (minX < minY) {
        // std::cout << "Before resolve: p=(" << p.x << "," << p.y << "," << p.w << "," << p.h
        //         << ") w=(" << w.x << "," << w.y << "," << w.w << "," << w.h << ")\n";
        // std::cout << "overlapLeft=" << overlapLeft
        //         << " overlapRight=" << overlapRight
        //         << " overlapTop=" << overlapTop
        //         << " overlapBottom=" << overlapBottom << "\n";
        if (overlapLeft < overlapRight) {
            t.position.x -= overlapLeft;
        } else {
            t.position.x += overlapRight;
        }
        // std::cout << "After resolve transform=(" << t.position.x << "," << t.position.y << ") "
        //         << " collider=(" << cPlayer.rect.x << "," << cPlayer.rect.y << ")\n";
        v.direction.x = 0;
    } else {
        if (overlapTop < overlapBottom) {
            t.position.y -= overlapTop;
            a.isGrounded = true;
        } else {
            t.position.y += overlapBottom;
        }

        v.direction.y = 0;
    }

    cPlayer.rect.x = t.position.x + cPlayer.offset.x;
    cPlayer.rect.y = t.position.y + cPlayer.offset.y;
}
