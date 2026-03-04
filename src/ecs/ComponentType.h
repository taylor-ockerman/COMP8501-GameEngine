//
// Created by taylo on 1/21/2026.
//

#ifndef INC_8051TUTORIAL_COMPONENTTYPE_H
#define INC_8051TUTORIAL_COMPONENTTYPE_H
#include <cstddef>


//creating a type alias for std::size_t
//size_t is designed to represent sizes and indices because its unsigned and its very large (long long)
using ComponentTypeID = std::size_t;

//when you have a free function and defined in a header, each .cpp file that includes this header
//would get its own definition causing a linking error, using inline merges them into one definition
//global counter generator, returns a unique number everytime we call it
//making id static, means it will keep its value through multiple calls of this function
inline ComponentTypeID getComponentTypeID() {
    static ComponentTypeID id = 0;
    return id++;
}

//can overload this function because they have different signatures
//template allows us to use generic types
//template is inline by default
//getComponentTypeID<Position>() -> would always return 0
//getComponentTypeID<Health>() -> would always return 1
//a static local variable is created and initialized once
template<typename T>
ComponentTypeID getComponentTypeID() {
    static ComponentTypeID typeID = getComponentTypeID();
    return typeID;
}
#endif //INC_8051TUTORIAL_COMPONENTTYPE_H
