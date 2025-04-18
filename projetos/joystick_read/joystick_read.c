#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"

#define SDA_PIN 14
#define SCL_PIN 15
#define VRX_ADC 27  // ADC1
#define VRY_ADC 26  // ADC0

int main() {
    stdio_init_all();
    sleep_ms(2000);  // Aguarda conexão com terminal

    // Inicializa I2C para o display OLED
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    ssd1306_init();

    struct render_area frame_area = {
        .start_column = 0,
        .end_column = ssd1306_width - 1,
        .start_page = 0,
        .end_page = ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);
    uint8_t buffer[ssd1306_buffer_length];
    memset(buffer, 0, sizeof(buffer));
    render_on_display(buffer, &frame_area);

    // Inicializa ADC
    adc_init();
    adc_gpio_init(VRY_ADC);  // ADC0
    adc_gpio_init(VRX_ADC);  // ADC1

    while (true) {
        adc_select_input(0);
        uint16_t vry = adc_read();  // GPIO26

        adc_select_input(1);
        uint16_t vrx = adc_read();  // GPIO27

        // Mostra no terminal
        printf("VRx: %4d\tVRy: %4d\n", vrx, vry);

        // Mostra no OLED
        char linha1[20], linha2[20];
        sprintf(linha1, "VRx: %4d", vrx);
        sprintf(linha2, "VRy: %4d", vry);

        memset(buffer, 0, sizeof(buffer));
        ssd1306_draw_string(buffer, 0, 0, "Leitura Joystick:");
        ssd1306_draw_string(buffer, 0, 16, linha1);
        ssd1306_draw_string(buffer, 0, 32, linha2);
        render_on_display(buffer, &frame_area);

        sleep_ms(200);
    }

    return 0;
}
