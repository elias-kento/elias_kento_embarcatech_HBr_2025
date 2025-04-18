#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"

// Pinos da BitDogLab
#define SDA_PIN 14
#define SCL_PIN 15
#define BUTTON_A 5
#define BUTTON_B 6

// Variáveis globais
volatile int contador = 0;
volatile int cliques_b = 0;
volatile bool contando = false;

// Área de renderização do display
struct render_area frame_area = {
    .start_column = 0,
    .end_column = ssd1306_width - 1,
    .start_page = 0,
    .end_page = ssd1306_n_pages - 1
};

uint8_t buffer[ssd1306_buffer_length];

// Atualiza o display OLED com o estado atual
void atualizar_display() {
    memset(buffer, 0, sizeof(buffer));
    char linha1[32], linha2[32];
    sprintf(linha1, "Contador: %d", contador);
    sprintf(linha2, "Cliques B: %d", cliques_b);
    ssd1306_draw_string(buffer, 0, 0, "Contagem:");
    ssd1306_draw_string(buffer, 0, 16, linha1);
    ssd1306_draw_string(buffer, 0, 32, linha2);
    render_on_display(buffer, &frame_area);
}

// Interrupção compartilhada dos botões
void gpio_callback(uint gpio, uint32_t events) {
    if (gpio == BUTTON_A && (events & GPIO_IRQ_EDGE_FALL)) {
        contador = 9;
        cliques_b = 0;
        contando = true;
        atualizar_display();
    }

    if (gpio == BUTTON_B && (events & GPIO_IRQ_EDGE_FALL)) {
        if (contando && contador > 0) {
            cliques_b++;
            atualizar_display();
        }
    }
}

// Timer: decrementa o contador a cada segundo
bool callback_timer(struct repeating_timer *t) {
    if (contando && contador > 0) {
        contador--;
        atualizar_display();
        if (contador == 0) {
            contando = false; // Parar a contagem ao chegar a zero
        }
    }
    return true;
}

int main() {
    stdio_init_all();
    sleep_ms(1000); // Aguarda conexão com terminal

    // Inicializa I2C para o display OLED
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
    ssd1306_init();
    calculate_render_area_buffer_length(&frame_area);
    atualizar_display();

    // Configura botão A
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    // Configura botão B
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    // Configura interrupção compartilhada
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled(BUTTON_B, GPIO_IRQ_EDGE_FALL, true);

    // Cria o timer repetitivo de 1 segundo
    struct repeating_timer timer;
    add_repeating_timer_ms(-1000, callback_timer, NULL, &timer);

    while (true) {
        tight_loop_contents(); // Sistema ocioso, controlado por interrupções
    }

    return 0;
}
