#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"

// Pinos padrão do I2C da BitDogLab
#define I2C_SDA 14
#define I2C_SCL 15

// Converte valor lido no ADC para temperatura em °C
float read_internal_temp_celsius() {
    const float conversion_factor = 3.3f / (1 << 12); // 12 bits
    adc_select_input(4); // Canal do sensor interno
    uint16_t raw = adc_read();
    float voltage = raw * conversion_factor;
    float temperature = 27.0f - (voltage - 0.706f) / 0.001721f;
    return temperature;
}

int main() {
    stdio_init_all();
    sleep_ms(2000);  // Aguarda conexão USB

    // Inicializa I2C para o display
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init();

    struct render_area frame_area = {
        .start_column = 0,
        .end_column = ssd1306_width - 1,
        .start_page = 0,
        .end_page = ssd1306_n_pages - 1
    };
    calculate_render_area_buffer_length(&frame_area);
    uint8_t buffer[ssd1306_buffer_length];

    // Inicializa ADC e sensor interno
    adc_init();
    adc_set_temp_sensor_enabled(true);

    while (true) {
        float temp_c = read_internal_temp_celsius();

        // Terminal
        printf("Temperatura interna: %.2f C\n", temp_c);

        // Display
        memset(buffer, 0, sizeof(buffer));
        ssd1306_draw_string(buffer, 0, 0, "Temp. interna:");
        char linha[32];
        sprintf(linha, "%.2f C", temp_c);
        ssd1306_draw_string(buffer, 0, 16, linha);
        render_on_display(buffer, &frame_area);

        sleep_ms(1000);
    }

    return 0;
}
