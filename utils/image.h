#pragma once

#include <fstream>
#include <iostream> 


class Color {
public: 
    uint8_t r, g, b; 
    
    Color() {r = 0; g = 0; b = 0;}

    Color(uint8_t _r, uint8_t _g, uint8_t _b);
}

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
    }

    void drawCircle(Vec2 &center, int radius, Color color) {
        size_t xstart = max(0, radius - center.x);
        size_t xend = min(w - 1, radius + center.x);
        size_t xstart = max(0, radius - center.y);
        size_t xend = min(h - 1, radius + center.y);
        for(size_t x = xstart; x < xend; x++) {
            for(size_t y = ystart; y < yend; y++) {
                size_t i = w * y + x;
                data[i] = color; 
            }
        }
    }

    bool exportFile(const std::string &path) {
        if (currFrame >= maxFrames) {
            std::cout << "Reached max frame cap!" << endl;
            return;
        }

        std::string filename = path + std::to_string(currFrame) + ".ppm";

        // Write data into ppm output file
        ofstream output_file(filename.c_str(), ios::binary);
        
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
    }
}