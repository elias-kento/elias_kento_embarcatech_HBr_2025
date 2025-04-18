// Projeto: Contador decrescente com registro de eventos por interrupção
// Plataforma: BitDogLab com Raspberry Pi Pico (RP2040)

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "ssd1306.h" // Biblioteca para controle do display OLED (assumindo que você já tenha essa lib)

#define BUTTON_A 5
#define BUTTON_B 6
#define OLED_SDA 14
#define OLED_SCL 15

volatile int countdown = 0;
volatile int click_count = 0;
volatile bool counting = false;

// Função para atualizar o display com os valores
void update_display(ssd1306_t *oled, int count, int clicks) {
    char line1[20];
    char line2[20];
    sprintf(line1, "Counter: %d", count);
    sprintf(line2, "Clicks: %d", clicks);

    ssd1306_clear(oled);
    ssd1306_draw_string(oled, 0, 0, 1, line1);
    ssd1306_draw_string(oled, 0, 10, 1, line2);
    ssd1306_show(oled);
}

// Interrupção do Botão A (reinicia a contagem)
void gpio_callback_button_a(uint gpio, uint32_t events) {
    if (gpio == BUTTON_A && (events & GPIO_IRQ_EDGE_FALL)) {
        countdown = 9;
        click_count = 0;
        counting = true;
    }
}

// Interrupção do Botão B (registra clique apenas se estiver contando)
void gpio_callback_button_b(uint gpio, uint32_t events) {
    if (gpio == BUTTON_B && (events & GPIO_IRQ_EDGE_FALL)) {
        if (counting) {
            click_count++;
        }
    }
}

int main() {
    stdio_init_all();

    // Inicializa os botões
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_callback_button_a);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_callback_button_b);

    // Inicializa o OLED com SoftI2C (supondo uso da lib ssd1306 adaptada)
    ssd1306_t oled;
    ssd1306_init(&oled, OLED_SDA, OLED_SCL, 128, 64);

    update_display(&oled, countdown, click_count);

    while (1) {
        if (counting) {
            update_display(&oled, countdown, click_count);
            sleep_ms(1000);
            countdown--;
            if (countdown < 0) {
                countdown = 0;
                counting = false;  // Para de contar e ignora novos cliques
                update_display(&oled, countdown, click_count);
            }
        } else {
            sleep_ms(100); // Idle loop
        }
    }
}
