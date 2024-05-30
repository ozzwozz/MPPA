#include "adc.h"

ADC::ADC()
{
    adc_init();
    adc_gpio_init(supply_13V_pin);
}

ADC::~ADC()
{
}

void ADC::read_P5V0_as_V(float &voltage)
{
    adc_select_input(P5V0_pin);
    // convert raw value to V
    voltage = (3.3 * adc_read()) / (1 << 12);
}

void ADC::read_P3V3_as_V(float &voltage)
{
    adc_select_input(P5V0_pin);
    // convert raw value to V
    voltage = (3.3 * adc_read()) / (1 << 12);
}
