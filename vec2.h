#pragma once
#include <cmath>

// TODO should add methods to choose a random vector in a direction
// could be useful to have ones which are weighted (to make it more likely to move in current direction of travel).

class Vec2 {
public:
    int x;
    int y;

    double l2() {
        return sqrt(x * x + y * y);
    }

    int l1() {
        return x + y;
    }

    Vec2 operator-(const Vec2& v) {
        Vec2 vec;
        vec.x = x - v.x;
        vec.y = y - v.y;

        return vec;
    }
}

Vec2::Vec2(int _x = 0, int _y = 0) {
    x = _x;
    y = _y;
}