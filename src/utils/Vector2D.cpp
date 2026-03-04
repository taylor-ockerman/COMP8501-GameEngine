//
// Created by taylo on 1/28/2026.
//

#include "../utils/Vector2D.h"

#include <math.h>

//member operator function so we can use the Vetocr2D on the left hand side
Vector2D Vector2D::operator*(float scalar) const {
    return {x * scalar, y * scalar};
}

//non-member operator function so we make use of putting the vector2D on the right hand side
Vector2D operator*(float scalar, Vector2D &v) {
    return {v.x * scalar, v.y * scalar};
}

//member operator functions so we can use the vector 2D that we want
//to change on the left hand side
Vector2D &Vector2D::operator+=(const Vector2D &vec) {
    this->x += vec.x;
    this->y += vec.y;
    return *this;
}

Vector2D &Vector2D::normalize() {
    float length = sqrt(x * x + y * y);
    if (length > 0) {
        this->x /= length;
        this->y /= length;
    }
    return *this;
}

Vector2D Vector2D::operator/(float scalar) const {
    return {x / scalar, y / scalar};
}

Vector2D operator/(float scalar, Vector2D &vec) {
    return {vec.x / scalar, vec.y / scalar};
}

Vector2D &Vector2D::operator-=(const Vector2D &vec) {
    this->x -= vec.x;
    this->y -= vec.y;
    return *this;
}

Vector2D Vector2D::operator+(const Vector2D &vec) {
    return {x + vec.x, y + vec.y};
}

Vector2D Vector2D::operator-(const Vector2D &vec) {
    return {x - vec.x, y - vec.y};
}

bool operator==(const Vector2D &vec1, const Vector2D &vec2) {
    if (vec1.x == vec2.x && vec1.y == vec2.y) {
        return true;
    }
    return false;
}

Vector2D &Vector2D::operator-() {
    this->x = -this->x;
    this->y = -this->y;
    return *this;
}
