#pragma once

#include "pico/stdlib.h"
#include "hardware/gpio.h"

/// @class PSU
/// @brief PSU Driver Class - Control functions for controlling a PA
class PSU
{
    private:
        /// @param m_enable_pin power enable pin
        uint m_enable_pin;
        /// @param m_shutdown_pin PA shutdown pin
        uint m_shutdown_pin;
                
    public:
        /// @brief PSU Class Constructor
        /// @param enable_pin 
        /// @param m_shutdown_pin 
        PSU(uint enable_pin, uint m_shutdown_pin);
        /// @brief PSU Class Desctructor
        ~PSU();

        /// @brief enable the PA
        void pa_enable();
        /// @brief disable the PA
        void pa_disable();
        /// @brief status of the PA
        bool pa_status();
        /// @brief shutdown the PA
        void pa_shutdown();
        /// @brief Allow the PA to be turned on
        void pa_turn_on();
        /// @brief check teh power state of the PA
        /// @return 
        bool pa_power_state();
};
