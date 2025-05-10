#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"

#define SDA_PIN 14
#define SCL_PIN 15
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

ssd1306_t tela;
uint8_t buffer[ssd1306_buffer_length];

typedef struct {
    int x;
    int y;
    bool ativa;
} Bola;

Bola bola;

void inicializar_bola() {
    bola.x = DISPLAY_WIDTH / 2;
    bola.y = 0;
    bola.ativa = true;
}

void atualizar_bola() {
    if (bola.ativa) {
        bola.y += 2;
        if (bola.y % 8 == 0) {
            bola.x += (rand() % 3) - 1; // -1, 0 ou +1 (desvio aleatório)
        }
        if (bola.x < 0) bola.x = 0;
        if (bola.x >= DISPLAY_WIDTH) bola.x = DISPLAY_WIDTH - 1;
        if (bola.y >= DISPLAY_HEIGHT - 1) {
            bola.ativa = false;
        }
    }
}

void desenhar_bola() {
    if (bola.ativa) {
        ssd1306_set_pixel(buffer, bola.x, bola.y, true);
    }
}

int main() {
    stdio_init_all();
    sleep_ms(2000);

    // Inicializa I2C e OLED
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    ssd1306_init_bm(&tela, 128, 64, false, 0x3C, i2c1);
    ssd1306_config(&tela);
    tela.ram_buffer = buffer;

    inicializar_bola();

    while (true) {
        memset(buffer, 0, ssd1306_buffer_length);

        atualizar_bola();
        desenhar_bola();

        ssd1306_send_data(&tela);

        if (!bola.ativa) {
            sleep_ms(500);
            inicializar_bola();
        }

        sleep_ms(100);
    }

    return 0;
}
