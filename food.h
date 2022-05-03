#pragma once
#include "utils/vec2.h"


class Food {
public:
    Vec2 pos;
    bool eaten;
    int value;

    Food(Vec2 _pos, int _value) {
        pos = _pos;
        value = _value;
        eaten = false;
    }

    // sets food position and also resets its eaten status 
    void setPosition(int _x, int _y) {
        pos = Vec2(_x, _y);
        eaten = false;
    }

};