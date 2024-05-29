#include <stdio.h>
#include "pico/stdlib.h"

#include "adc.h"
#include "AD7490BCPZ.h"
#include "DS1682.h"
#include "LED.h"
#include "M24M02.h"
#include "PSU.h"
#include "UART_Handler.h"

#define SPI_MISO_PIN 16
#define SPI_MOSI_PIN 19
#define SPI_SCK_PIN 18
#define SPI_CS_PIN 17

#define EEPROM_ADDR 0x50
#define DS1682_ADDR 0xD6

int main()
{
    stdio_init_all();

    spi_init(spi0, 1'000'000);
    gpio_set_function(SPI_MISO_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI_MOSI_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI_CS_PIN, GPIO_FUNC_SPI);

    spi_set_format(spi0, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

    LED pa1_led = LED(15);
    LED pa2_led = LED(20);
    LED pa3_led = LED(21);
    LED pa4_led = LED(22);
    LED pa5_led = LED(26);
    
    PSU psu_1 = PSU(9, 2);
    PSU psu_2 = PSU(10, 3);
    PSU psu_3 = PSU(11, 6);
    PSU psu_4 = PSU(12, 7);
    PSU psu_5 = PSU(13, 8);

    M24M02 m24m02 = M24M02(i2c0, EEPROM_ADDR);
    ADC adc = ADC();
    DS1682 ds1682 = DS1682(i2c0, DS1682_ADDR);
    AD7490BCPZ ad7490 = AD7490BCPZ(spi0, SPI_CS_PIN);

    UART_Handler uart_handler = UART_Handler(uart1, 9600, 9, 8, psu_1, psu_2, psu_3, psu_4, psu_5
                                            , m24m02, adc, ds1682, ad7490);

    while (true) {
        if (psu_1.pa_power_state())
        {
            pa1_led.LED_on();
        }
        if (psu_2.pa_power_state())
        {
            pa2_led.LED_on();
        }
        if (psu_3.pa_power_state())
        {
            pa3_led.LED_on();
        }
        if (psu_4.pa_power_state())
        {
            pa4_led.LED_on();
        }
        if (psu_5.pa_power_state())
        {
            pa5_led.LED_on();
        }

        sleep_ms(50);
         
        if (psu_1.pa_status())
        {
            pa1_led.LED_off();
        }
        if (psu_2.pa_status())
        {
            pa2_led.LED_off();
        }
        if (psu_3.pa_status())
        {
            pa3_led.LED_off();
        }
        if (psu_4.pa_status())
        {
            pa4_led.LED_off();
        }
        if (psu_5.pa_status())
        {
            pa5_led.LED_off();
        }
    }
}
