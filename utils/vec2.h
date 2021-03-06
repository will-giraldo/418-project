#pragma once
#include <cmath>

// TODO should add methods to choose a random vector in a direction
// could be useful to have ones which are weighted (to make it more likely to move in current direction of travel).

class Vec2 {
public:
    int x;
    int y;

    Vec2(int _x = 0, int _y = 0);

    friend std::ostream& operator<< (std::ostream &out, const Vec2& v);

    double l2() {
        return sqrt(x * x + y * y);
    }

    int l1() {
        return x + y;
    }

    // casts to a valid direction in an integer grid
    Vec2 toDir() {
        Vec2 vec;
        double len = this->l2();
        double _x = ((double) x) / len;
        double _y = ((double) y) / len;
        vec.x = (int) std::round(_x);
        vec.y = (int) std::round(_y);

        return vec;
    }

    Vec2 operator-(const Vec2& v) {
        Vec2 vec;
        vec.x = x - v.x;
        vec.y = y - v.y;

        return vec;
    }

    Vec2 operator+(const Vec2& v) {
        Vec2 vec;
        vec.x = x + v.x;
        vec.y = y + v.y;

        return vec;
    }

    Vec2 operator*(const int& a) {
        Vec2 vec;
        vec.x = a * x;
        vec.y = a * y;

        return vec; 
    }

    bool operator==(const Vec2& v) {
        return v.x == x && v.y == y;
    }

    bool operator!=(const Vec2& v) {
        return v.x != x || v.y != y;
    }


};

std::ostream& operator<< (std::ostream &out, const Vec2& v) {
        out << "< " << v.x << ", " << v.y << " >";
        return out;
}

Vec2::Vec2(int _x, int _y) {
    x = _x;
    y = _y;
}