#include "leds.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define GREEN_PWM_LED 11
#define BLUE_PWM_LED 12
#define RED_PWM_LED 13
#define CLOCK_DIVIDER 2.0
#define WRAP 2048
#define MIN_BRIGHTNESS 0

static volatile bool active = true; 

static void init_pin(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(pin);
    pwm_set_clkdiv(slice, CLOCK_DIVIDER);
    pwm_set_wrap(slice, WRAP);
    pwm_set_gpio_level(pin, MIN_BRIGHTNESS);
    pwm_set_enabled(slice, true);
}

void leds_init() {
    init_pin(GREEN_PWM_LED);
    init_pin(BLUE_PWM_LED);
    init_pin(RED_PWM_LED);
}

static void set_level(uint led_pin, float brightness) {
    float _brightness = brightness >= 0 ? brightness : -1 * brightness;
    uint level = WRAP * _brightness;
    pwm_set_gpio_level(led_pin, active ? level : MIN_BRIGHTNESS);
}

void leds_set_green_brightness(float brightness) {
    set_level(GREEN_PWM_LED, brightness);
}

void leds_set_blue_brightness(float brightness) {
    set_level(BLUE_PWM_LED, brightness);
}

void leds_set_red_brightness(float brightness) {
    set_level(RED_PWM_LED, brightness);
}

void leds_toggle() {
    active = !active;
}