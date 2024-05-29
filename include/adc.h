#pragma once

#include "pico/stdlib.h"
#include "hardware/adc.h"

/// @class ADC
/// @brief ADC Driver for reading from analogue to digital pins
class ADC
{
    private:
        uint P5V0_pin = 28;
        uint P3V3_pin = 27;
    public:
        /// @brief Construct a new ADC object
        ADC();
        /// @brief Destroy the ADC object
        ~ADC();

        /// @param supply_13V_pin 13V supply pin
        const uint supply_13V_pin = 20;
        /// @param PA_PGOOD_13V_pin 13V PA PGOOD pin
        const uint PA_PGOOD_13V_pin = 1;

        /// @brief enable the 13V supply line
        void read_P5V0_as_V(float &voltage);

        void read_P3V3_as_V(float &voltage);
};
