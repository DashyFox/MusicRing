#include <Arduino.h>
//#include <avr/iom328.h>

#include "modules/LED_Ring.h"
#include "misc/bitmaps.h"
#include "implement/Display__Adafruit_SSD1306.h"

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
    static bool f = false;
    if (millis() - tmr > 350) {
        screen.drawBitmap(105, 23, f ? DashyFox_logo_point_1 : DashyFox_logo_point_2, 16, 3);
        screen.adafruit->display();
        tmr = millis();
        f = !f;
    }

}