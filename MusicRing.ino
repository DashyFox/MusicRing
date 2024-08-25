#include "Arduino.h"
//#include <avr/iom328.h>

#include "./modules/LED_Ring.h"
#include "./implement/Display__Adafruit_SSD1306.h"
#include "./misc/bitmaps.h"

LED_Ring_PINOUT pinout { PA4, PA3, PA5, PA1, PA2 };

LED_Ring ring(32, pinout);

void ISR() { ring.isr(); }

void setup() {
    Serial.begin(115200);

    ring.begin(ISR);





}


void loop() {
    if (Serial.available())
    {
        uint8_t input = Serial.parseInt();
        ring.setFrec(input);
    }
    

}