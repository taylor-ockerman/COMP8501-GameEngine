//
// Created by taylo on 3/25/2026.
//

#ifndef INC_8051TUTORIAL_PARTICLEHELPERS_H
#define INC_8051TUTORIAL_PARTICLEHELPERS_H
#include "Component.h"

class ParticleHelpers {
public:
    static ParticleProperties getProperties(ParticleType type) {
        switch (type) {
            case ParticleType::Empty:
                return {0, ParticleBehaviour::Static};
            case ParticleType::Sand:
                return {20, ParticleBehaviour::Powder};
            case ParticleType::Stone:
                return {100, ParticleBehaviour::Static};
            case ParticleType::Fire:
                return {1, ParticleBehaviour::Gas};
            case ParticleType::Water:
                return {10, ParticleBehaviour::Liquid};
            case ParticleType::Gunpowder:
                return {15, ParticleBehaviour::Powder};
            case ParticleType::Smoke:
                return {5, ParticleBehaviour::Gas};
            default:
                return {0, ParticleBehaviour::Static};
        }
    }
};
#endif //INC_8051TUTORIAL_PARTICLEHELPERS_H
