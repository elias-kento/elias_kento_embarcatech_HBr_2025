#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define DELAY 5000           // em microssegundos
#define LED 13            // GPIO 13 (GP13)
#define MAXBRIGHT 0xFF    // brilho máximo (255)

int main() {
    stdio_init_all();

    gpio_init(LED);
    gpio_set_function(LED, GPIO_FUNC_PWM);

    // Obtém o slice PWM correspondente ao pino 13
    uint slice_num = pwm_gpio_to_slice_num(LED);

    // Configura o topo do contador PWM para permitir valores mais altos (até 65535)
    pwm_set_wrap(slice_num, 65535);

    // Habilita o PWM
    pwm_set_enabled(slice_num, true);

    while (true) {
        printf("Hello, world!!!\n");

        // Fade in
        for (int pwdLvl = 0; pwdLvl <= MAXBRIGHT; pwdLvl++) {
            uint16_t level = pwdLvl * pwdLvl;
            pwm_set_gpio_level(LED, level);
            sleep_us(DELAY);
        }

        // Fade out
        for (int pwdLvl = MAXBRIGHT; pwdLvl >= 0; pwdLvl--) {
            uint16_t level = pwdLvl * pwdLvl;
            pwm_set_gpio_level(LED, level);
            sleep_us(DELAY);
        }
    }
}
