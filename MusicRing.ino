#include <Arduino.h>
//#include <avr/iom328.h>

#include "implement/Display__Adafruit_SSD1306.h"
#include "misc/bitmaps.h"

Display_Adafruit_SSD1306 screen(128, 32, Display::Ori_HORIZONTAL); 

#define pin_Data 5          // yellow
#define pin_CLK_inside 6    // orange
#define pin_CLK_outside 7   // red

#define pin_RESET 8         // green
#define pin_OutputEnable 9  // purple

#define LED_count 32


void shift(uint16_t, bool = true);

volatile uint8_t position = 1;  // позиция сдвига
volatile uint16_t frec = 42;    // частота MusicRing
volatile uint16_t t_period_takts = F_CPU / 8 / LED_count / frec;

void setup() {

    Serial.begin(115200);

    pinMode(pin_Data, OUTPUT);
    pinMode(pin_CLK_inside, OUTPUT);
    pinMode(pin_CLK_outside, OUTPUT);
    pinMode(pin_RESET, OUTPUT);
    pinMode(pin_OutputEnable, OUTPUT);

    screen.begin();
    
    screen.adafruit->drawBitmap(0, 0, DashyFox_logo, 128, 32, WHITE);
    screen.adafruit->display();

    digitalWrite(pin_RESET, HIGH);

    reset();


    for (size_t j = 0; j < 1; j++) {
        for (size_t i = 1; i <= LED_count * 2; i++) {
            digitalWrite(pin_Data, i <= LED_count ? HIGH : LOW);
            shift(1);
            delay(11);
        }
    }

    delay(100);

    for (size_t i = 0; i < 3; i++) {
        uint8_t del = 42;
        digitalWrite(pin_Data, HIGH);
        shift(LED_count);
        delay(del * 1.5);
        reset();
        delay(del);
    }

    // инициализация Timer1
    cli(); // отключить глобальные прерывания
    TCCR1A = 0; // установить регистры в 0
    TCCR1B = 0;

    OCR1A = t_period_takts; // установка регистра совпадения
    TCCR1B |= (1 << WGM12); // включение в CTC режим

    // Установка битов CS на коэффициент деления 8
    TCCR1B |= 0b00000010;

    TIMSK1 |= (1 << OCIE1A);  // включение прерываний по совпадению
    sei(); // включить глобальные прерывания
}

ISR(TIMER1_COMPA_vect) {
    if (position >= LED_count) {
        point_ini(1);
        position = 1;
    } else shift(1);
}

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

void point_ini(uint8_t width) {

    reset();

    digitalWrite(pin_Data, HIGH);
    shift(width);
    digitalWrite(pin_Data, LOW);

}

void shift(uint16_t count, bool isVisible = true) {
    for (size_t i = 0; i < count; i++) {

        digitalWrite(pin_CLK_inside, HIGH);
        if (isVisible) digitalWrite(pin_CLK_outside, HIGH);

        digitalWrite(pin_CLK_inside, LOW);
        digitalWrite(pin_CLK_outside, LOW);

        position++;
    }
};

void reset() {
    digitalWrite(pin_RESET, LOW);
    digitalWrite(pin_RESET, HIGH);
    digitalWrite(pin_CLK_outside, HIGH);
    digitalWrite(pin_CLK_outside, LOW);
}