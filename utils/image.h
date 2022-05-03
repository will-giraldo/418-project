#pragma once

#include <fstream>
#include <iostream> 
#include <sstream>

#include "vec2.h"


class Color {
public: 
    uint8_t r, g, b; 
    
    Color() {r = 0; g = 0; b = 0;}

    Color(uint8_t _r, uint8_t _g, uint8_t _b) {
        r = _r;
        g = _g;
        b = _b;
    }
};

class Image {
public: 
    size_t w, h;
    Color *data;

    Image() {}
    Image(size_t _w, int _h) {}

    size_t currFrame;
    size_t maxFrames = 500; 

    bool clearImage() {
        for(size_t y = 0; y < h; y++) {
            size_t row = w * y;
            for(size_t x = 0; x < w; x++) {
                data[row + x] = Color(255, 255, 255);
            }
        }
        return true;
    }

    void drawCircle(Vec2 &center, int radius, Color color) {
        size_t xstart = std::max(0, radius - center.x);
        size_t xend = std::min(w - 1, (size_t)(radius + center.x));
        size_t ystart = std::max(0, radius - center.y);
        size_t yend = std::min(h - 1, (size_t)(radius + center.y));
        for(size_t x = xstart; x < xend; x++) {
            for(size_t y = ystart; y < yend; y++) {
                size_t i = w * y + x;
                data[i] = color; 
            }
        }
    }

    bool exportFile(const std::string &path) {
        if (currFrame >= maxFrames) {
            std::cout << "Reached max frame cap!" << std::endl;
            return false;
        }

        std::string filename = path + std::to_string(currFrame) + ".ppm";

        // Write data into ppm output file
        std::ofstream output_file(filename.c_str(), std::ios::binary);
        
        for(size_t y = 0; y < h; y++) {
            size_t row = w * y;
            for(size_t x = 0; x < w; x++) {
                Color color = data[row + x];
                char dat[3] = {color.r, color.g, color.b};
                output_file.write(dat, 3);
            }
        }

        currFrame++;
        output_file.close();
        return true;
    }
};