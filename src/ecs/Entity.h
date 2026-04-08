//
// Created by taylo on 1/21/2026.
//

#ifndef INC_8051TUTORIAL_ENTITY_H
#define INC_8051TUTORIAL_ENTITY_H
#include <array>
#include <bitset>
#include <cstddef>

#include "ComponentType.h"

//create a const that is evaluated at compile time (constexpr)
//32 components fits perfectly in a 32-bit integer
constexpr std::size_t MAX_COMPONENTS = 32;

//bitset[0] = 1; //entity has component 0
//bit operations are fast and work well for checking each frame
using ComponentBitSet = std::bitset<MAX_COMPONENTS>;

//fixed size array, where each index is a component
//the index corresponds to the ComponentTypeID
//we use a fixed number of components so there is no dynamic allocation for the array (efficiency)
using ComponentArray = std::array<void *, MAX_COMPONENTS>;

class Entity {
    bool active = true;
    ComponentArray componentArray{};
    ComponentBitSet componentBitSet{};

public:
    bool isActive() { return active; }

    void destroy() {
        active = false;
    }

    //templates need to be implemented in the header
    //entity.hasComponent(position)
    template<typename T>
    bool hasComponent() {
        return componentBitSet[getComponentTypeID<T>()];
    }

    //template that can receive multiple arguments
    //entity.addComponent<Position>(4,5)
    //entity.addComponent<Health>(100)
    template<typename T, typename... mArgs>
    T &addComponent(mArgs &&... args) {
        //implements perfect forwarding
        //everything is not treated as a lvalue
        //this is more efficient with moves and temporaries
        T *component(new T{std::forward<mArgs>(args)...});
        //add the new component to the array
        componentArray[getComponentTypeID<T>()] = component;
        //update the bitset to true
        componentBitSet[getComponentTypeID<T>()] = true;
        return *component;
    }

    template<typename T>
    T &getComponent() {
        auto ptr(componentArray[getComponentTypeID<T>()]);
        return *static_cast<T *>(ptr);
    }

    template<typename T>
    void deactivateComponent() {
        componentBitSet[getComponentTypeID<T>()] = false;
    }
};

#endif //INC_8051TUTORIAL_ENTITY_H
