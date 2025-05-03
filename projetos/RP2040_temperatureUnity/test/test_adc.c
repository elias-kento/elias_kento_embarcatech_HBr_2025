#include "unity.h"
#include "adc_temp.h"

void setUp(void) {}
void tearDown(void) {}

void test_adc_to_celsius_known_value(void) {
    uint16_t adc_simulado = 876;
    float temp = adc_to_celsius(adc_simulado);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 27.0f, temp);
}
