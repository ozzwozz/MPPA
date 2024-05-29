#include "UART_Handler.h"
#include <string.h>
#include <stdio.h>

UART_Handler::UART_Handler(uart_inst_t *uart, uint baud_rate, uint rx_pin, uint tx_pin
        , PSU &psu_1, PSU &psu_2, PSU &psu_3, PSU &psu_4, PSU &psu_5
        , M24M02 &m24m02, ADC &adc, DS1682 &ds1682, AD7490BCPZ &AD7490BCPZ)
            : m_uart(uart)
            , m_m24m02(m24m02)
            , m_adc(adc)
            , m_psu_1(psu_1)
            , m_psu_2(psu_2)
            , m_psu_3(psu_3)
            , m_psu_4(psu_4)
            , m_psu_5(psu_5)
            , m_ds1682(ds1682)
            , m_ads8166(AD7490BCPZ)
{
    gpio_set_function(rx_pin, GPIO_FUNC_UART);
    gpio_set_function(tx_pin, GPIO_FUNC_UART);
    uart_set_baudrate(m_uart, baud_rate);
    uart_set_format(m_uart, 8, 1, UART_PARITY_NONE);

    // Turn off FIFO's - we want to do this character by character
    uart_set_fifo_enabled(m_uart, false);

    // Set UART_Handler flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(m_uart, false, false);

    uart_set_irq_enables(m_uart, true, false);
    
    // check which instance of UART_Handler is being used to give the correct UART_Handler interrupt handler
    int UART_IRQ = uart == m_uart ? UART0_IRQ : UART1_IRQ;
    irq_set_exclusive_handler(UART_IRQ, (irq_handler_t)uart_irq_handler);
    irq_set_enabled(UART_IRQ, true);
}

void UART_Handler::write(const char *data)
{
    uart_write_blocking(m_uart, reinterpret_cast<const uint8_t*>(data), strlen(data));
}

void UART_Handler::write(const char *data, size_t len)
{
    uart_write_blocking(m_uart, reinterpret_cast<const uint8_t*>(data), len);
}

size_t UART_Handler::read(char *data, size_t len)
{
    size_t bytes_read = 0;
    while (!rx_buffer_.empty() && bytes_read < len)
    {
        data[bytes_read++] = rx_buffer_.front();
        rx_buffer_.pop();
    }
    return bytes_read > 0;
}

bool UART_Handler::available()
{
    return uart_is_readable(m_uart) ? 1 : 0;
}

void UART_Handler::flush_rx()
{
    rx_buffer_ = std::queue<char>();
}

void UART_Handler::flush_tx()
{
    tx_buffer_ = std::queue<std::vector<char>>();
}

void UART_Handler::uart_irq_handler(void *context)
{
    UART_Handler *uart = static_cast<UART_Handler *>(context);

    while (uart_is_readable(uart->m_uart))
    {
        char c = uart_getc(uart->m_uart);
        uart->rx_buffer_.push(c);
    }
}

// TODO: Issue #4 - Finish implementation of decoding and responding to UART messages
void UART_Handler::decode_message()
{
    char data[128]; // Allocate memory for the data buffer

    if (!read(data, sizeof(data)))
    {
        return;
    }

    printf("message: %s\n", data);

    uint8_t header = data[0];

    std::vector<char> response;

    response[0] = header;

    switch (header)
    {
        case message_headers::SET_PSU:
            set_psu(data);
            break;
        case message_headers::GET_PSU:
            get_psu(response, data[1]);
            break;
        case message_headers::SET_PA_ENABLE:
            set_pa_enable(data);
            break;
        case message_headers::GET_PA_ENABLE:
            get_pa_enable(response);
            break;
        case message_headers::SET_CHARACTERISATION:
            set_characterisation(data);
            break;
        case message_headers::GET_CHARACTERISATION:
            get_characterisation(response);
            break;
        case message_headers::GET_BITS:
            get_bits(response);
            break;
        case message_headers::GET_HARDWARE_NUMBERS:
            get_hardware_numbers(response);
            break;
        case message_headers::GET_SOFTWARE_NUMBERS:
            get_software_numbers(response);
            break;
        default:
            break;
    }

    tx_buffer_.push(response);
}

// TODO: Issue #4 - Finish Implementation of decoding and responding to UART messages
size_t UART_Handler::send_message()
{
    size_t bytes_sent = 0;
    char data[128];
    while (!tx_buffer_.empty() && bytes_sent < sizeof(data))
    {
        // data[bytes_sent++] = tx_buffer_.front();
        tx_buffer_.pop();
    }

    write(data);

    return bytes_sent > 0;
}

void UART_Handler::set_psu(char* data)
{
    uint8_t band_mask = data[2];

    if ((band_mask & (1 << 7)) != 0)
    {
        m_psu_1.pa_enable();
    }
    if ((band_mask & (1 << 6)) != 0)
    {
        m_psu_2.pa_enable();
    }
    if ((band_mask & (1 << 5)) != 0)
    {
        m_psu_3.pa_enable();
    }
    if ((band_mask & (1 << 4)) != 0)
    {
        m_psu_4.pa_enable();
    }
    if ((band_mask & (1 << 3)) != 0)
    {
        m_psu_5.pa_enable();
    }
}

void UART_Handler::get_psu(std::vector<char>& response, uint8_t band_mask)
{
    uint8_t psu_status;

    psu_status |= (m_psu_1.pa_status() << 7);
    psu_status |= (m_psu_2.pa_status() << 6);
    psu_status |= (m_psu_3.pa_status() << 5);
    psu_status |= (m_psu_4.pa_status() << 4);
    psu_status |= (m_psu_5.pa_status() << 3);

    response[1] = psu_status;
}

void UART_Handler::set_pa_enable(char* data)
{
    uint8_t band_mask = data[2];

    if ((band_mask & (1 << 7)) != 0)
    {
        m_psu_1.pa_enable();
    }
    if ((band_mask & (1 << 6)) != 0)
    {
        m_psu_2.pa_enable();
    }
    if ((band_mask & (1 << 5)) != 0)
    {
        m_psu_3.pa_enable();
    }
    if ((band_mask & (1 << 4)) != 0)
    {
        m_psu_4.pa_enable();
    }
    if ((band_mask & (1 << 3)) != 0)
    {
        m_psu_5.pa_enable();
    }
}

void UART_Handler::get_pa_enable(std::vector<char>& response)
{
    uint8_t lna_status;
    bool value;

    lna_status |= (m_psu_1.pa_status() << 7);
    lna_status |= (m_psu_1.pa_status() << 6);
    lna_status |= (m_psu_1.pa_status() << 5);
    lna_status |= (m_psu_1.pa_status() << 4);
    lna_status |= (m_psu_1.pa_status() << 3);
    
    response[1] = lna_status;
}

// TODO: Issue #3 Implement undefined 
void UART_Handler::set_characterisation(char* data)
{
    uint8_t calibration_table = data[1];
    uint8_t band_mask = data[2];
}

void UART_Handler::get_characterisation(std::vector<char>& response)
{
    // unknown return
}

void UART_Handler::get_bits(std::vector<char>& response)
{
    uint32_t timestamp;
    float temperature;

    size_t initialSize = response.size(); // Store initial size to calculate the size of added data

    // Get timestamp from DS1682
    if (m_ds1682.getTime(timestamp))
    {
        response.resize(initialSize + sizeof(uint32_t));
        memcpy(response.data() + initialSize, &timestamp, sizeof(uint32_t));
    }
}

void UART_Handler::get_hardware_numbers(std::vector<char>& response)
{
    uint32_t device_id;

    // ETR Hardware Number
    response[1] = m_ds1682.getUniqueID(device_id);

    // EEPROM (?)
}

void UART_Handler::get_software_numbers(std::vector<char>& response)
{
    // Pi Pico Software Number
}
