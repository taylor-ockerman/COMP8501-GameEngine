//
// Created by taylo on 3/25/2026.
//

#ifndef INC_8051TUTORIAL_PARTICLEHELPERS_H
#define INC_8051TUTORIAL_PARTICLEHELPERS_H
#include <unordered_map>

#include "Component.h"

class ParticleHelpers {
public:
    static ParticleProperties getProperties(ParticleType type, bool getRandomSprite) {
        switch (type) {
            case ParticleType::Empty:
                return {0, ParticleBehaviour::Static, randomTile(type, tilesetLocations.at(type), getRandomSprite)};
            case ParticleType::Sand:
                return {20, ParticleBehaviour::Powder, randomTile(type, tilesetLocations.at(type), getRandomSprite)};
            case ParticleType::Stone:
                return {100, ParticleBehaviour::Static, randomTile(type, tilesetLocations.at(type), getRandomSprite)};
            case ParticleType::Fire:
                return {1, ParticleBehaviour::Liquid, randomTile(type, tilesetLocations.at(type), getRandomSprite)};
            case ParticleType::Water:
                return {10, ParticleBehaviour::Liquid, randomTile(type, tilesetLocations.at(type), getRandomSprite)};
            case ParticleType::Gunpowder:
                return {15, ParticleBehaviour::Powder, randomTile(type, tilesetLocations.at(type), getRandomSprite)};
            case ParticleType::Smoke:
                return {5, ParticleBehaviour::Gas, randomTile(type, tilesetLocations.at(type), getRandomSprite)};
            case ParticleType::Wood:
                return {50, ParticleBehaviour::Static, randomTile(type, tilesetLocations.at(type), getRandomSprite)};
            case ParticleType::Oil:
                return {15, ParticleBehaviour::Liquid, randomTile(type, tilesetLocations.at(type), getRandomSprite)};
            case ParticleType::Steam:
                return {1, ParticleBehaviour::Gas, randomTile(type, tilesetLocations.at(type), getRandomSprite)};
            case ParticleType::Wall:
                return {999, ParticleBehaviour::Static, randomTile(type, tilesetLocations.at(type), getRandomSprite)};
            default:
                return {0, ParticleBehaviour::Static, randomTile(type, tilesetLocations.at(type), getRandomSprite)};
        }
    }

private:
    static SDL_FRect randomTile(ParticleType type, SDL_FRect src, bool getRandom) {
        if (getRandom) {
            int colorVariant = pickVariant(type);
            src.y += (colorVariant / 2) * 64;
            src.x += (colorVariant % 2) * 64;
        }
        return src;
    }

    static int weightedVariant(int w0, int w1, int w2, int w3) {
        int total = w0 + w1 + w2 + w3;
        int roll = rand() % total;

        if (roll < w0) return 0;
        roll -= w0;

        if (roll < w1) return 1;
        roll -= w1;

        if (roll < w2) return 2;

        return 3;
    }

    static int pickVariant(ParticleType type) {
        switch (type) {
            case ParticleType::Sand:
                // randomize evenly
                return weightedVariant(25, 25, 25, 25);

            case ParticleType::Stone:
                // mostly middle two shades
                return weightedVariant(2, 48, 48, 2);

            case ParticleType::Water:
                // bias toward darker shades
                return weightedVariant(40, 5, 50, 5);

            case ParticleType::Smoke:
                // lighter overall
                return weightedVariant(35, 10, 45, 10);

            case ParticleType::Steam:
                // even lighter overall
                return weightedVariant(1, 1, 54, 54);

            case ParticleType::Wood:
                // mostly mid tones
                return weightedVariant(10, 40, 40, 10);

            case ParticleType::Fire:

                return weightedVariant(35, 35, 20, 10);

            case ParticleType::Oil:
                // mostly darkest two
                //return 0;
                return weightedVariant(50, 30, 15, 5);
            case ParticleType::Erase:
                return 0;
            default:
                return 0;
        }
    }

    static inline const std::unordered_map<ParticleType, SDL_FRect> tilesetLocations = {
        {ParticleType::Empty, SDL_FRect{512, 512, 0, 0}},
        {ParticleType::Sand, SDL_FRect{0, 0, 64, 64}},
        {ParticleType::Stone, SDL_FRect{128, 0, 64, 64}},
        {ParticleType::Water, SDL_FRect{256, 0, 64, 64}},
        {ParticleType::Smoke, SDL_FRect{384, 0, 64, 64}},
        {ParticleType::Wood, SDL_FRect{0, 128, 64, 64}},
        {ParticleType::Fire, SDL_FRect{128, 128, 64, 64}},
        {ParticleType::Oil, SDL_FRect{256, 128, 64, 64}},
        {ParticleType::Steam, SDL_FRect{384, 128, 64, 64}},
        {ParticleType::Wall, SDL_FRect{448, 0, 64, 64}},
        {ParticleType::Erase, SDL_FRect{0, 256, 64, 64}}
    };
};
#endif //INC_8051TUTORIAL_PARTICLEHELPERS_H
