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
    uint8_t* screenBuffer;
    uint16_t screenBufferSize;

public:
    Display(uint16_t width, uint16_t height, uint16_t orientation) {
        screen.width = width;
        screen.height = height;
        screenBufferSize = (width * height) / 8 + ((width * height) / 8) ? 1 : 0;
        screen.orientation = orientation;

        screenBuffer = new uint8_t[screenBufferSize];
    }

    virtual void writePixel(uint16_t x, uint16_t y, bool color) {};

    void drawBitmap(int16_t posX, int16_t posY, const uint8_t bitmap [], int16_t sizeX, int16_t sizeY) {

        for (uint16_t iY = 1; iY <= sizeY; iY++) {
            for (uint16_t iX = 1; iX <= sizeX; iX++) {

                uint16_t local = iY * (sizeY - 1) + iX;
                uint16_t global = screen.height * (posY + iY - 1) + (posX + iX);
                
                bool localPixel = (bitmap[local / 8] >> (7 - (local % 8))) & 1;
                bool globalPixel = (screenBuffer[global / 8] >> (7 - (global % 8))) & 1;

                if (localPixel != globalPixel) {
                    _writePixel(posX + iX, posY + iY, localPixel);
                }
            }
        }
    }

protected:
    void _writePixel(uint16_t x, uint16_t y, bool color) {
        uint16_t Y = screen.height * (y - 1);
        if (color) {
            screenBuffer[(Y + x) / 8] |= (1 << (7 - (Y + x) % 8)); // |= 0b00010000
        } else {
            screenBuffer[(Y + x) / 8] &= ~(1 << (7 - (Y + x) % 8)); // &= 11101111
        }
        writePixel(x, y, color);
    }
};
