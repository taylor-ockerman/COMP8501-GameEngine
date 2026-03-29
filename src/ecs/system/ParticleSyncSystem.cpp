
//
// Created by taylo on 3/29/2026.
//
#include "ParticleSyncSystem.h"
#include "World.h"
#include "ParticleGrid.h"

void ParticleSyncSystem::update(ParticleGrid &grid) {
    int cellSize = grid.getCellSize();
    for (int y = 0; y < grid.getHeight(); y++) {
        for (int x = 0; x < grid.getWidth(); x++) {
            Cell &cell = grid.at(x, y);

            if (cell.type == ParticleType::Empty || cell.entity == nullptr)continue;
            if (!cell.entity->hasComponent<Particle>() || !cell.entity->hasComponent<Transform>()) continue;

            auto &particle = cell.entity->getComponent<Particle>();
            auto &transform = cell.entity->getComponent<Transform>();

            particle.gridX = x;
            particle.gridY = y;

            transform.oldPosition = transform.position;
            transform.position.x = static_cast<float>(x * cellSize);
            transform.position.y = static_cast<float>(y * cellSize);

            if (cell.entity->hasComponent<Collider>()) {
                auto &collider = cell.entity->getComponent<Collider>();
                collider.rect.x = transform.position.x + collider.offset.x;
                collider.rect.y = transform.position.y + collider.offset.y;
                collider.rect.w = (float) cellSize;
                collider.rect.h = (float) cellSize;
            }

            if (cell.entity->hasComponent<Sprite>()) {
                auto &sprite = cell.entity->getComponent<Sprite>();
                sprite.dst.x = transform.position.x;
                sprite.dst.y = transform.position.y;
                sprite.dst.w = (float) cellSize;
                sprite.dst.h = (float) cellSize;
            }
        }
    }
}
