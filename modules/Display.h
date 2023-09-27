#pragma once
#include <Arduino.h>

class Display {
public:
    enum ScreenOrientation {
        Ori_0 = 0,
        Ori_90 = 90,
        Ori_180 = 180,
        Ori_270 = 270,

        Ori_HORIZONTAL = 0,
        Ori_VERTICAL = 90,
        Ori_HORIZONTAL_FLIP = 180,
        Ori_VERTICAL_FLIP = 270,
    };

    template<uint16_t X, uint16_t Y>
    struct Picture {
        uint16_t sizeX = X;
        uint16_t sizeY = Y;
        uint8_t bitMap[(X * Y) / 8 + ((X * Y) / 8) ? 1 : 0];
        uint8_t overMap[(X * Y) / 8 + ((X * Y) / 8) ? 1 : 0];
    };


protected:
    struct ScreenParam {
        uint16_t width, height, orientation;
    } screen;

    uint8_t* buffer;
public:
    Display(uint16_t width, uint16_t height, uint16_t orientation) {
        screen.width = width;
        screen.height = height;
        screen.orientation = orientation;

        buffer = new uint8_t[((width * height) / 8)];
    }

    virtual void writePixel() {};

    void drawBbitmap(int16_t x, int16_t y, const uint8_t bitmap [],
        int16_t w, int16_t h) {

        Picture<128, 32> logo;
    }



};
