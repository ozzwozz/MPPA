# == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
   set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(PICO_SDK_PATH ${USERHOME}/.pico-sdk/sdk/1.5.1)
set(PICO_TOOLCHAIN_PATH ${USERHOME}/.pico-sdk/toolchain/13_2_Rel1)
if(WIN32)
    set(pico-sdk-tools_DIR ${USERHOME}/.pico-sdk/tools/1.5.1)
    include(${pico-sdk-tools_DIR}/pico-sdk-tools-config.cmake)
    include(${pico-sdk-tools_DIR}/pico-sdk-tools-config-version.cmake)
endif()
# ====================================================================================
# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
   set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(PICO_SDK_PATH ${USERHOME}/.pico-sdk/sdk/1.5.1)
set(PICO_TOOLCHAIN_PATH ${USERHOME}/.pico-sdk/toolchain/13_2_Rel1)
if(WIN32)
    set(pico-sdk-tools_DIR ${USERHOME}/.pico-sdk/tools/1.5.1)
    include(${pico-sdk-tools_DIR}/pico-sdk-tools-config.cmake)
    include(${pico-sdk-tools_DIR}/pico-sdk-tools-config-version.cmake)
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(m_hpssm_controller C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Header files here
set(HEADER_FILES
    include/adc.h
    include/AD7490BCPZ.h
    include/DS1682.h
    include/I2CDevice.h
    include/LED.h
    include/M24M02.h
    include/MAX31725.h
    include/PSU.h
    include/SPIDevice.h
    include/UART_Handler.h
)

# Source files here
set(SOURCE_FILES
    src/adc.cpp
    src/AD7490BCPZ.cpp
    src/DS1682.cpp
    src/I2CDevice.cpp
    src/LED.cpp
    src/MAX31725.cpp
    src/M24M02.cpp
    src/m_hpssm_controller.cpp
    src/PSU.cpp
    src/SPIDevice.cpp
    src/UART_Handler.cpp
)

# Include the directories containing the header files
include_directories(include)

# Add executable. Default name is the project name, version 0.1
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

pico_set_program_name(m_hpssm_controller "m_hpssm_controller")
pico_set_program_version(m_hpssm_controller "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(m_hpssm_controller 1)
pico_enable_stdio_usb(m_hpssm_controller 0)

# Add the standard library to the build
target_link_libraries(m_hpssm_controller
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(m_hpssm_controller PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # common lwipopts or any other standard includes, if required
)

# Requested libraries
target_link_libraries(m_hpssm_controller 
        hardware_i2c
        hardware_watchdog
        hardware_adc
        hardware_spi
        )

pico_add_extra_outputs(m_hpssm_controller)
