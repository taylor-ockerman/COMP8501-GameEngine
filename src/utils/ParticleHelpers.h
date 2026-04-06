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
                return {
                    0, false, 0, 0, ParticleBehaviour::Static,
                    getTileSprite(type, tilesetLocations.at(type), getRandomSprite)
                };
            case ParticleType::Sand:
                return {
                    20, false, 100, 0, ParticleBehaviour::Powder,
                    getTileSprite(type, tilesetLocations.at(type), getRandomSprite)
                };
            case ParticleType::Stone:
                return {
                    100, false, 100, 0, ParticleBehaviour::Static,
                    getTileSprite(type, tilesetLocations.at(type), getRandomSprite)
                };
            case ParticleType::Fire:
                return {
                    100, false, 200, 0, ParticleBehaviour::Static,
                    getTileSprite(type, tilesetLocations.at(type), getRandomSprite)
                };
            case ParticleType::Water:
                return {
                    10, false, 100, 100, ParticleBehaviour::Liquid,
                    getTileSprite(type, tilesetLocations.at(type), getRandomSprite)
                };
            case ParticleType::Gunpowder:
                return {
                    15, true, 5, 100, ParticleBehaviour::Powder,
                    getTileSprite(type, tilesetLocations.at(type), getRandomSprite)
                };
            case ParticleType::Smoke:
                return {
                    5, false, 1000, 0, ParticleBehaviour::Gas,
                    getTileSprite(type, tilesetLocations.at(type), getRandomSprite)
                };
            case ParticleType::Wood:
                return {
                    50, true, 400, 3, ParticleBehaviour::Static,
                    getTileSprite(type, tilesetLocations.at(type), getRandomSprite)
                };
            case ParticleType::Oil:
                return {
                    21, true, 30, 5, ParticleBehaviour::Liquid,
                    getTileSprite(type, tilesetLocations.at(type), getRandomSprite)
                };
            case ParticleType::Steam:
                return {
                    1, false, 1000, 0, ParticleBehaviour::Gas,
                    getTileSprite(type, tilesetLocations.at(type), getRandomSprite)
                };
            case ParticleType::Wall:
                return {
                    999, false, 100, 0, ParticleBehaviour::Static,
                    getTileSprite(type, tilesetLocations.at(type), getRandomSprite)
                };
            default:
                return {
                    0, false, 100, 0, ParticleBehaviour::Static,
                    getTileSprite(type, tilesetLocations.at(type), getRandomSprite)
                };
        }
    }

    static SDL_FRect getTileSprite(ParticleType type, SDL_FRect src, bool getRandom) {
        if (getRandom) {
            int colorVariant = pickVariant(type);
            src.y += (colorVariant / 2) * 64;
            src.x += (colorVariant % 2) * 64;
        }
        return src;
    }

    static std::string particleTypeToString(ParticleType type) {
        switch (type) {
            case ParticleType::Empty: return "Empty";
            case ParticleType::Sand: return "Sand";
            case ParticleType::Stone: return "Stone";
            case ParticleType::Smoke: return "Smoke";
            case ParticleType::Water: return "Water";
            case ParticleType::Gunpowder: return "Gunpowder";
            case ParticleType::Fire: return "Fire";
            case ParticleType::Wall: return "Wall";
            case ParticleType::Oil: return "Oil";
            case ParticleType::Wood: return "Wood";
            case ParticleType::Steam: return "Steam";
            case ParticleType::Erase: return "Erase";
            default: return "Unknown";
        }
    }

    static std::string spawnClipForParticle(ParticleType type) {
        switch (type) {
            case ParticleType::Sand: return "powderSpawn";
            case ParticleType::Water: return "waterSpawn";
            case ParticleType::Fire: return "fireSpawn";
            default: return "";
        }
    }

private:
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
                return weightedVariant(25, 25, 25, 25);
            case ParticleType::Stone:
                return weightedVariant(2, 48, 48, 2);
            case ParticleType::Water:
                return weightedVariant(40, 5, 50, 5);
            case ParticleType::Smoke:
                return weightedVariant(35, 10, 45, 10);
            case ParticleType::Steam:
                return weightedVariant(1, 1, 54, 54);
            case ParticleType::Wood:
                return weightedVariant(10, 40, 40, 10);
            case ParticleType::Fire:
                return weightedVariant(35, 35, 20, 10);
            case ParticleType::Oil:
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
