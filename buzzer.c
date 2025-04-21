#include "buzzer.h"
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define BUZZER 21
#define WRAP 12500
#define LEVEL 6250

static uint slice;
static volatile bool active = true; 

void buzzer_init() {
    gpio_set_function(BUZZER, GPIO_FUNC_PWM);
    slice = pwm_gpio_to_slice_num(BUZZER);
    pwm_set_clkdiv(slice, 1);
    pwm_set_wrap(slice, WRAP);
    pwm_set_gpio_level(BUZZER, LEVEL);
    pwm_set_enabled(slice, active);
}

static uint frequency(float relative_frequency) {
    return (uint) round(relative_frequency + 1);
}

static uint absolute_frequency(float x_relative_frequency, float y_relative_frequency) {
    return 3 * frequency(-y_relative_frequency) + frequency(x_relative_frequency) + 1;
}

void buzzer_buzzee(float x_relative_frequency, float y_relative_frequency) {
    uint frequency = absolute_frequency(x_relative_frequency, y_relative_frequency);
    pwm_set_clkdiv(slice, frequency);
}

void buzzer_toggle() {
    active = !active;
    pwm_set_enabled(slice, active);
}