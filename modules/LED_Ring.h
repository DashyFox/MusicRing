#pragma once
#include <Arduino.h>

struct LED_Ring_PINOUT {
    uint8_t
        Data,           // yellow
        CLK_inside,     // red
        CLK_outside,    // orange
        RESET,          // green
        OutputEnable;   // pur
};

class LED_Ring {
private:
    HardwareTimer *MyTim = nullptr;

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

    void begin(void(*func)(void)) {

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

        timerIni(frec, func);
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
        if (position > ledCount) {
            point_ini(1);
            position = 1;
        } else shift(1);
    }

    //////////////////////////////////////////////////////////////////////////////

void timerIni(uint16_t _frec, void(*func)(void)) {
    // Определяем таймер, например, используем TIM2
    MyTim = new HardwareTimer(TIM2);

    // Рассчитываем период
    uint32_t prescaler = 8; // Делитель
    period = (SystemCoreClock / (prescaler * _frec)) - 1;

    // Останавливаем таймер перед настройкой
    MyTim->pause();

    // Настраиваем предделитель (делитель частоты)
    MyTim->setPrescaleFactor(prescaler);
    
    // Настраиваем таймер в режиме CTC (count-to-clear)
    MyTim->setOverflow(period);
    MyTim->attachInterrupt(func);
    MyTim->resume();
}

void setFrec(uint16_t frec) {
    uint32_t prescaler = 8; // Делитель
    period = (SystemCoreClock / (prescaler * frec)) - 1;

    // Останавливаем таймер перед изменением периода
    MyTim->pause();
    MyTim->setOverflow(period);
    MyTim->refresh(); // Обновление настроек таймера
    MyTim->resume();
}

void setPeriod(uint32_t per) {
    period = per;

    // Останавливаем таймер перед изменением периода
    MyTim->pause();
    MyTim->setOverflow(period);
    MyTim->refresh(); // Обновление настроек таймера
    MyTim->resume();
}


    /////////////////////////////////////////////////////////////////
};
