//
// Created by taylo on 1/28/2026.
//

#ifndef INC_8051TUTORIAL_VECTOR2D_H
#define INC_8051TUTORIAL_VECTOR2D_H

class Vector2D {
public:
    float x = 0.0f;
    float y = 0.0f;

    Vector2D() {
        x = 0.0f;
        y = 0.0f;
    }

    Vector2D(float x, float y) : x(x), y(y) {
    }

    //member operator function
    //Vector2D hast to be on the left, float on the right
    Vector2D operator*(float scalar) const;

    //Vector2D on the right, and float on the left
    //need to make this a non-member function
    friend Vector2D operator*(float scalar, Vector2D &vec);

    //member operator function
    //Vector2D hast to be on the left, float on the right
    Vector2D operator/(float scalar) const;

    //Vector2D on the right, and float on the left
    //need to make this a non-member function
    friend Vector2D operator/(float scalar, Vector2D &vec);

    //vector that we want to return on the left
    //another vector is on the right
    Vector2D &operator+=(const Vector2D &vec);

    Vector2D &operator-=(const Vector2D &vec);

    Vector2D operator+(const Vector2D &vec);

    Vector2D operator-(const Vector2D &vec);

    friend bool operator==(const Vector2D &vec1, const Vector2D &vec2);

    Vector2D &operator-();

    Vector2D &normalize();
};
#endif //INC_8051TUTORIAL_VECTOR2D_H
