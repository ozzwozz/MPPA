#pragma once

#include "pico/stdlib.h"
#include "SPIDevice.h"

/// @class AD7490BCPZ
/// @brief AD7490BCPZ Driver for being used for temperature sensing
class AD7490BCPZ : public SPIDevice
{
    union ad7490_cmd_value
    {
        struct
        {
            uint16_t                         : 4;
            uint16_t    linear_binary_coding : 1;
            uint16_t    single_range         : 1;
            uint16_t    dout_weak_pull       : 1;
            uint16_t    shadow               : 1;
            uint16_t    power_management     : 2;

            uint16_t    convert_address      : 4;
            uint16_t    sequence             : 1;
            uint16_t    write                : 1;
        } B;            ///< Bit field accessor
        uint16_t I;     ///< Integer accessor
    };
    
    private:
        
    public:
        /// @brief AD7490BCPZ Class Constructor
        /// @param spi_inst spi instance
        /// @param cs_pin chip select pin
        AD7490BCPZ(spi_inst_t* spi_inst, uint cs_pin);

        /// @brief AD7490BCPZ Class Destructor
        ~AD7490BCPZ();

        /// @brief Read analogue input on given channel
        /// @param channel channel to read
        /// @return temperature on channel
        uint16_t read(const uint8_t channel);
};
