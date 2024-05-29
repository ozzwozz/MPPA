#pragma once

#include "I2CDevice.h"


/// @class MAX31725 - Local Temperature Sensor Driver
/// @brief Driver for the MAX31725
class MAX31725 : public I2CDevice
{
    private:
        uint8_t cmd_read_temperature = 0x00;
        uint m_overtemp_pin = 14;

    public:
        /// @brief Construct a new MAX31725 object
        /// @param i2c i2c instance
        /// @param address Address of the target
        MAX31725(i2c_inst_t *i2c, uint8_t address);

        /// @brief Destroy the MAX31725 object
        ~MAX31725();

        /// @brief Read register storing the temperature data on the MAX31725
        /// @param temperature get the temperature by reference
        /// @return success is true
        bool read_temperature(float &temperature);

        /// @brief Handler for the over temp interrupt
        static void over_temp_irq_handler(void *context);
};