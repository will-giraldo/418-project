#pragma once
#include "utils/vec2.h"


class Food {
public:
    Vec2 pos;
    bool eaten;

    Food(Vec2 _pos) {
        pos = _pos;
        eaten = false;
    }

};