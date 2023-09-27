#include <Arduino.h>
//#include <avr/iom328.h>

#include "modules/LED_Ring.h"
#include "implement/Display__Adafruit_SSD1306.h"
#include "misc/bitmaps.h"

LED_Ring_PINOUT pinout { 5, 6, 7, 8, 9 };

LED_Ring ring(32, pinout);
Display_Adafruit_SSD1306 screen(128, 32, Display::Ori_HORIZONTAL);

void setup() {
    Serial.begin(115200);

    ring.begin();
    screen.begin();

    screen.adafruit->drawBitmap(0, 0, DashyFox_logo, 128, 32, WHITE);
    screen.adafruit->display();

}

ISR(TIMER1_COMPA_vect) { ring.isr(); }

void loop() {

    static uint32_t tmr;
    static uint8_t  arr[6] = { 0x3f >> 1, 0xf0 ,0x7f >> 3, 0xf8, 0x3f >> 1, 0xf0 };
    static uint8_t arr2[6] = { 0 };
    static bool f = false;
    if (millis() - tmr > 350) {
        screen.adafruit->drawBitmap(105, 23, arr, 16, 3, f);
        screen.adafruit->display();
        tmr = millis();
        f = !f;
    }

}