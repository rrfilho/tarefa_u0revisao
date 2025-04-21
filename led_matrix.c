#include "led_matrix.h"
#include <math.h>
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "pio_matrix.h"
#include <stdio.h>

#define PIXELS 25
#define COLOR 6
#define FIRST_BYTE 24
#define SECOND_BYTE 16
#define THIRD_BYTE 8
#define LED_PIN 7
#define ON 286331136
#define OFF 0
static const uint MAP[PIXELS] = {
    24,23,22,21,20,
    15,16,17,18,19,
    14,13,12,11,10,
     5, 6, 7, 8, 9,
     4, 3, 2, 1, 0
};
static const PIO PIO_ID = pio0;
static uint sm;
static volatile bool active = true;

void led_matrix_init() {
    set_sys_clock_khz(128000, false);
    uint offset = pio_add_program(PIO_ID, &pio_matrix_program);
    sm = pio_claim_unused_sm(PIO_ID, true);
    pio_matrix_program_init(PIO_ID, sm, offset, LED_PIN);
}

static uint position(float relative_position) {
    return (uint) round(2*(relative_position + 1));
}

static uint absolute_position(float x_relative_position, float y_relative_position) {
    return 5 * position(-y_relative_position) + position(x_relative_position);
}

void led_matrix_draw(float x_relative_position, float y_relative_position) {
    uint position = absolute_position(x_relative_position, y_relative_position);
    for (uint i = 0; i < PIXELS; i++) {
        uint led_value = active && (MAP[i] == position) ? ON : OFF;
        pio_sm_put_blocking(PIO_ID, sm, led_value);
    }
}

void led_matrix_toggle() {
    active = !active;
}