#pragma once
#include <Arduino.h>

struct LED_Ring_PINOUT {
    uint8_t Data, CLK_inside, CLK_outside, RESET, OutputEnable;
};

class LED_Ring {
public:
    LED_Ring_PINOUT pinOut;
    uint16_t ledCount;
    uint16_t updateFrec;

    LED_Ring(uint16_t _ledCount, LED_Ring_PINOUT _pinOut) : ledCount(_ledCount), pinOut(_pinOut) {};

    
};
