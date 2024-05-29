#include "AD7490BCPZ.h"

AD7490BCPZ::AD7490BCPZ(spi_inst_t* spi_inst, uint cs_pin) : SPIDevice(spi_inst, cs_pin)
{
}

AD7490BCPZ::~AD7490BCPZ()
{
    // Should default us to master, but just being doubly sure
    spi_set_slave(spi0, false);
}

uint16_t AD7490BCPZ::read(const uint8_t channel)
{
    // Bitwise OR to set lower 4 bits to the channel number
    uint8_t tx_data[2] {0x80 | (channel << 4), 0x00};
    uint8_t rx_data[2];

    exchange(tx_data, rx_data, 2);

    return (rx_data[0] << 8) | rx_data[1];
}
