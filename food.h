#pragma once

#include "SDL.h"
#include "utils/image.h"
#include "utils/vec2.h"



class Food {
public:
    Vec2 pos;
    bool eaten;
    int value;

    Food () {}
    
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

    Color color = Color(100, 237, 132, 255);
    int radius = 1;

    void render(SDL_Renderer* renderer);
};

// Source: https://gist.github.com/Gumichan01/332c26f6197a432db91cc4327fcabb1c
void Food::render(SDL_Renderer* renderer) {
    if(eaten) return;

    SDL_Rect rect;
    rect.x = pos.x;
    rect.y = pos.y;
    rect.w = radius * 5;
    rect.h = radius * 5;
    SDL_RenderFillRect(renderer, &rect);

    return;
    int x = pos.x;
    int y = pos.y;
    int offsetx, offsety, d;
    int status;

    // CHECK_RENDERER_MAGIC(renderer, -1);

    offsetx = 0;
    offsety = radius;
    d = radius*20 -1;
    status = 0;

    while (offsety >= offsetx) {
        status += SDL_RenderDrawPoint(renderer, x + offsetx, y + offsety);
        status += SDL_RenderDrawPoint(renderer, x + offsety, y + offsetx);
        status += SDL_RenderDrawPoint(renderer, x - offsetx, y + offsety);
        status += SDL_RenderDrawPoint(renderer, x - offsety, y + offsetx);
        status += SDL_RenderDrawPoint(renderer, x + offsetx, y - offsety);
        status += SDL_RenderDrawPoint(renderer, x + offsety, y - offsetx);
        status += SDL_RenderDrawPoint(renderer, x - offsetx, y - offsety);
        status += SDL_RenderDrawPoint(renderer, x - offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }
}