#include "joystick.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"

static const uint JOYSTICK_X_AXIS = 27;
static const uint JOYSTICK_X_SELECTOR = 1;
static const uint JOYSTICK_Y_AXIS = 26;
static const uint JOYSTICK_Y_SELECTOR = 0;
static const float JOYSTICK_CENTER_VALUE = 2048.0f;

void joystick_init() {
    adc_init();
    adc_gpio_init(JOYSTICK_X_AXIS);
    adc_gpio_init(JOYSTICK_Y_AXIS);
}

static float joystick_read(uint selector) {
    adc_select_input(selector);
    sleep_us(2);
    uint value = adc_read();
    return value/JOYSTICK_CENTER_VALUE - 1.0f;
}

float joystick_read_x_axis() {
    return joystick_read(JOYSTICK_X_SELECTOR);
}

float joystick_read_y_axis() {
    return joystick_read(JOYSTICK_Y_SELECTOR);
}