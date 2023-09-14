

#define pin_Data 5          // yellow
#define pin_CLK_inside 6    // orange
#define pin_CLK_outside 7   // red

#define pin_RESET 8         // green
#define pin_OutputEnable 9  // purple

#define LED_count 32


void shift(uint16_t, bool = true);

uint8_t position = 1;
void setup() {
    pinMode(pin_Data, OUTPUT);
    pinMode(pin_CLK_inside, OUTPUT);
    pinMode(pin_CLK_outside, OUTPUT);
    pinMode(pin_RESET, OUTPUT);
    pinMode(pin_OutputEnable, OUTPUT);

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
        delay(del*1.5);
        reset();
        delay(del);
    }
}

void loop() {
    if (position >= LED_count) {
        point_ini(1);
        position = 1;
    }

    else shift(1);
    delay(1000/ 2 /LED_count); // Если больше 2х перейти на micros
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