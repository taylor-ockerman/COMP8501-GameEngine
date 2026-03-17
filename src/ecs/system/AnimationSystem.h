//
// Created by taylo on 2/18/2026.
//

#ifndef INC_8051TUTORIAL_ANIMATIONSYSTEM_H
#define INC_8051TUTORIAL_ANIMATIONSYSTEM_H
#include <iostream>
#include <memory>
#include <vector>
#include "../Entity.h"
#include "../Component.h"


//State system: Deciding which clip to use
//Check if the animation has been switched
//Playback system: advances the animation
class AnimationSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity> > &entities, float dt) {
        for (auto &e: entities) {
            if (e->hasComponent<Animation>() && e->hasComponent<Velocity>()) {
                auto &anim = e->getComponent<Animation>();
                auto &velocity = e->getComponent<Velocity>();

                //state system
                std::string newClip;
                std::string previousClip;
                if (velocity.direction.x > 0.0f) {
                    newClip = "walk_right";
                    if (e->hasComponent<PlayerTag>()) {
                        anim.lastMoveClip = newClip;
                    }
                } else if (velocity.direction.x < 0.0f) {
                    newClip = "walk_left";
                    if (e->hasComponent<PlayerTag>()) {
                        anim.lastMoveClip = newClip;
                    }
                } else if (velocity.direction.y < 0.0f) {
                    newClip = "walk_up";
                    if (e->hasComponent<PlayerTag>()) {
                        anim.lastMoveClip = newClip;
                    }
                } else if (velocity.direction.y > 0.0f) {
                    newClip = "walk_down";
                    if (e->hasComponent<PlayerTag>()) {
                        anim.lastMoveClip = newClip;
                    }
                } else {
                    if (anim.lastMoveClip == "walk_right") newClip = "idle_right";
                    else if (anim.lastMoveClip == "walk_left") newClip = "idle_left";
                    else if (anim.lastMoveClip == "walk_up") newClip = "idle_up";
                    else newClip = "idle_down";
                }

                //Check if the animation has switched
                //if the chosen clip is different from the current oen, switch to new clip, reset time and frame index
                if (newClip != anim.currentClip && e->hasComponent<PlayerTag>()) {
                    anim.currentClip = newClip;
                    anim.time = 0.0f;
                    anim.currentFrame = 0;
                }
                std::cout << "Anim address: " << &anim << "\n";
                std::cout << "Clip count: " << anim.clips.size() << "\n";
                std::cout << "Current clip: " << anim.currentClip << "\n";
                std::cout << "Clip exists: " << (anim.clips.find(anim.currentClip) != anim.clips.end()) << "\n\n";
                //Playback system: advances the animation
                float animFrameSpeed = anim.speed; //how long each animation frame should last
                auto clip = anim.clips[anim.currentClip]; //retrieve the frame data from the current clip
                //advance time
                anim.time += dt; //every "game loop frame" we are going to add the accumulated time

                //once enough time has passed
                if (anim.time >= animFrameSpeed) {
                    anim.time -= animFrameSpeed; //subtract animFrameSpeed (e.g. 0.1f) so the extra time isn't lost

                    std::size_t totalAnimationFrames = clip.frameIndices.size();
                    //advance to the next frame (currentAnimationFrame + 1)
                    // wrap around % so it loops when reaching the end of the clip
                    anim.currentFrame = (anim.currentFrame + 1) % totalAnimationFrames;
                }
            }
        }
    }
};

#endif //INC_8051TUTORIAL_ANIMATIONSYSTEM_H
