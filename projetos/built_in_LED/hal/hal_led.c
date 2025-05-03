#include "hal_led.h"
#include "led_embutido.h"
#include "pico/stdlib.h"

void hal_led_toggle(void) {
    led_embutido_on();
    sleep_ms(500);
    led_embutido_off();
    sleep_ms(500);
}
