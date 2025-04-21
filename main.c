#include <stdio.h>
#include "pico/stdlib.h"
#include "display.h"
#include "joystick.h"
#include "led_matrix.h"
#include "leds.h"
#include "buttons.h"
#include "buzzer.h"

volatile bool green_led_status = false;
volatile bool active = true;

void on_joystick_button_press() {
    green_led_status = !green_led_status;
    leds_set_green_brightness(green_led_status ? 0.5f : 0.0f);
    printf("O led verde foi %s.\n", green_led_status ? "ligado" : "desligado");
}

void on_left_button_press() {
    active = !active;
    led_matrix_toggle();
    leds_toggle();
    display_toggle();
    buzzer_toggle();
    on_joystick_button_press();
    printf("O sistema foi %s.\n", active ? "ativado" : "desativado");
}

void init() {
    stdio_init_all(); 
    joystick_init();
    display_init();
    led_matrix_init();
    leds_init();
    buttons_init(on_left_button_press, on_joystick_button_press);
    buzzer_init();
    printf("Aplicação Iniciada.\n");
}

int main() {
    init();
    while (true) {
        float x_relative_position = joystick_read_x_axis();
        float y_relative_position = joystick_read_y_axis();
        printf("Coordenadas Joystick: x= %f, y= %f\n", x_relative_position, y_relative_position);
        leds_set_red_brightness(x_relative_position);
        leds_set_blue_brightness(y_relative_position);
        display_draw_filled_square_and_rect(x_relative_position, y_relative_position, false);
        led_matrix_draw(x_relative_position, y_relative_position);
        buzzer_buzzee(x_relative_position, y_relative_position);
        sleep_ms(100);
    }
}
