#include "buttons.h"
#include "pico/stdlib.h"

#define LEFT_BUTTON 5
#define JOYSTICK_BUTTON 22
#define DEBOUNCE_TIME_LIMIT_US 200000

static handler _left_button_handler;
static handler _joystick_button_handler;
static volatile uint left_button_last_time_us = 0;
static volatile uint joystick_button_last_time_us = 0;

static uint now_us() {
    return to_us_since_boot(get_absolute_time());
}

static bool has_wait_time_reached(uint last_time) {
    return now_us() - last_time >= DEBOUNCE_TIME_LIMIT_US;
}

static void left_button_handler_with_debounce() {
    if (!has_wait_time_reached(left_button_last_time_us)) return;
    _left_button_handler();
    left_button_last_time_us = now_us();
}

static void joystick_button_handler_with_debounce() {
    if (!has_wait_time_reached(joystick_button_last_time_us)) return;
    _joystick_button_handler();
    joystick_button_last_time_us = now_us();
}

static void handler_adapter(uint gpio, uint32_t event_mask) {
    if (gpio == LEFT_BUTTON) left_button_handler_with_debounce();
    if (gpio == JOYSTICK_BUTTON) joystick_button_handler_with_debounce();
}

static void button_init(uint button_pin) {
    gpio_init(button_pin);
    gpio_set_dir(button_pin, GPIO_IN);
    gpio_pull_up(button_pin);
    gpio_set_irq_enabled_with_callback(button_pin, GPIO_IRQ_EDGE_FALL, true, handler_adapter);
}

void buttons_init(handler left_button_handler, handler joystick_button_handler) {
    _left_button_handler = left_button_handler;
    button_init(LEFT_BUTTON);
    _joystick_button_handler = joystick_button_handler;
    button_init(JOYSTICK_BUTTON);
}