#pragma once
#include <Arduino.h>

struct LED_Ring_PINOUT {
    uint8_t
        Data,
        CLK_inside,
        CLK_outside,
        RESET,
        OutputEnable;
};

class LED_Ring {
private:
    LED_Ring_PINOUT pinOut;
    uint16_t ledCount;
    uint16_t updateFrec;

    volatile uint16_t position = 1;     // позиция сдвига
    volatile uint16_t frec = 42;        // частота MusicRing
    volatile uint16_t period;           // период в тактах

public:
    LED_Ring(uint16_t _ledCount, LED_Ring_PINOUT _pinOut) : ledCount(_ledCount), pinOut(_pinOut) {

        period = F_CPU / 8 / ledCount / frec; //TODO: Динамически подстраивать разрешающую способность в зависимости от мк

        pinMode(pinOut.CLK_inside, OUTPUT);
        pinMode(pinOut.CLK_outside, OUTPUT);
        pinMode(pinOut.Data, OUTPUT);
        pinMode(pinOut.OutputEnable, OUTPUT);
        pinMode(pinOut.RESET, OUTPUT);

        digitalWrite(pinOut.RESET, HIGH);

    };

    void begin() {

        reset();

        for (size_t j = 0; j < 1; j++) {
            for (size_t i = 1; i <= ledCount * 2; i++) {
                digitalWrite(pinOut.Data, i <= ledCount ? HIGH : LOW);
                shift(1);
                delay(11);
            }
        }

        delay(100);

        for (size_t i = 0; i < 3; i++) {
            uint8_t del = 42;
            digitalWrite(pinOut.Data, HIGH);
            shift(ledCount);
            delay(del * 1.5);
            reset();
            delay(del);
        }

        timerIni(frec);
    }

    ///////////////////////////////////////////////////////////////////////////////////////

    void point_ini(uint8_t width) {

        reset();

        digitalWrite(pinOut.Data, HIGH);
        shift(width);
        digitalWrite(pinOut.Data, LOW);

    }

    void shift(uint16_t count, bool isVisible = true) {
        for (size_t i = 0; i < count; i++) {

            digitalWrite(pinOut.CLK_inside, HIGH);
            if (isVisible) digitalWrite(pinOut.CLK_outside, HIGH);

            digitalWrite(pinOut.CLK_inside, LOW);
            digitalWrite(pinOut.CLK_outside, LOW);

            position++;
        }
    };

    void reset() {
        digitalWrite(pinOut.RESET, LOW);
        digitalWrite(pinOut.RESET, HIGH);
        digitalWrite(pinOut.CLK_outside, HIGH);
        digitalWrite(pinOut.CLK_outside, LOW);
    }

    //////////////////////////////////////////////////////////////////////////////

    void isr() {
        if (position >= ledCount) {
            point_ini(1);
            position = 1;
        } else shift(1);
    }

    //////////////////////////////////////////////////////////////////////////////

    void timerIni(uint16_t _frec) { //TODO: Сделать возможность выбора таймеров
        period = F_CPU / 8 / ledCount / _frec;
        // инициализация Timer1
        cli(); // отключить глобальные прерывания
        TCCR1A = 0; // установить регистры в 0
        TCCR1B = 0;

        OCR1A = period; // установка регистра совпадения
        TCCR1B |= (1 << WGM12); // включение в CTC режим

        // Установка битов CS на коэффициент деления 8
        TCCR1B |= 0b00000010;

        TIMSK1 |= (1 << OCIE1A);  // включение прерываний по совпадению
        sei(); // включить глобальные прерывания
    }

    void setFrec(uint8_t frec) {
        period = F_CPU / 8 / ledCount / frec;
        cli();
        OCR1A = period; // установка регистра совпадения
        sei();
    }
    
    void setPeriod(uint8_t per) {
        period = per;
        cli();
        OCR1A = period; // установка регистра совпадения
        sei();
    }

    /////////////////////////////////////////////////////////////////
};
