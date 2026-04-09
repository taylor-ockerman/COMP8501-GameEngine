//
// Created by taylo on 3/29/2026.
//

#ifndef INC_8051TUTORIAL_PARTICLESYNCSYSTEM_H
#define INC_8051TUTORIAL_PARTICLESYNCSYSTEM_H
class World;
class ParticleGrid;

class ParticleSyncSystem {
public:
    void update(ParticleGrid &grid); //keeps entity transforms and sprite synced with grid positions
};
#endif //INC_8051TUTORIAL_PARTICLESYNCSYSTEM_H
