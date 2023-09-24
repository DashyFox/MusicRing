#pragma once
#include "../modules/Display.h"

#include <Adafruit_SSD1306.h>

class Display_Adafruit_SSD1306 : public Display {
public:
    Adafruit_SSD1306* adafruit;
    Display_Adafruit_SSD1306(uint16_t width, uint16_t height, uint16_t orientation) : Display(width, height, orientation) {
        adafruit = new Adafruit_SSD1306(width, height, &Wire, -1);
    };

    void begin() { //TODO: Добавить поддержку других режимов
        adafruit->begin(SSD1306_SWITCHCAPVCC, 0x3C);
        adafruit->clearDisplay();
    }


    void writePixel() override {

    }

    ~Display_Adafruit_SSD1306() {
        delete adafruit;
    }
};
