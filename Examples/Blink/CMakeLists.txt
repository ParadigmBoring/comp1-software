cmake_minimum_required(VERSION 3.15.3)

project(nucleo-f303re)

enable_language(CXX C ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(STM32CUBEMX_GENERATED_FILES

        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim.c
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim_ex.c
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_uart.c
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc.c
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc_ex.c
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash.c
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash_ex.c
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_gpio.c
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_dma.c
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr.c
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr_ex.c
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_cortex.c
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal.c
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_exti.c
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pcd.c
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pcd_ex.c
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_can.c
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_ll_usb.c
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c.c
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c_ex.c


        Core/Inc/main.h
        Core/Inc/stm32f3xx_hal_conf.h
        Core/Inc/stm32f3xx_it.h
        Core/Inc/can.h
        Core/Inc/gpio.h
        Core/Inc/usart.h
        Core/Inc/canTest.h
        Core/Inc/i2c.h
        Core/Inc/pwmTest.h

        Core/Src/main.cpp
        Core/Src/stm32f3xx_hal_msp.c
        Core/Src/stm32f3xx_it.c
        Core/Src/system_stm32f3xx.c
        Core/Src/can.c
        Core/Src/gpio.c
        Core/Src/usart.c
        Core/Src/canTest.cpp
        Core/Src/i2c.c
        Core/Src/pwmTest.cpp

        startup_stm32f303xe.s
        )

set(SOURCES
        # Add user-created source files to this variable
        ../../Subsurface/Node/Common/Peripherals/Src/can_interface.cpp
        ../../Subsurface/Node/Common/Peripherals/Src/can_manager.cpp
        ../../Subsurface/Node/Common/Peripherals/Src/convert_hal_status.cpp
        ../../Subsurface/Node/Common/Peripherals/src/SPIInterface.cpp
        ../../Subsurface/Node/Common/Peripherals/Src/I2CInterface.cpp
        ../../Subsurface/Node/Common/Peripherals/Src/PWMInterface.cpp
        ../../Subsurface/Node/Common/Peripherals/Src/PWMManager.cpp
        ../../Subsurface/Node/Common/Peripherals/Src/DPS310.cpp
)

set(EXECUTABLE ${PROJECT_NAME}.out)

add_executable(${EXECUTABLE} ${STM32CUBEMX_GENERATED_FILES} ${SOURCES})

target_compile_definitions(${EXECUTABLE} PRIVATE
        -DUSE_HAL_DRIVER
        -DSTM32F303xE
        )

target_include_directories(${EXECUTABLE} PRIVATE
        Core/Inc
        Drivers/STM32F3xx_HAL_Driver/Inc
        Drivers/CMSIS/Device/ST/STM32F3xx/Include
        Drivers/CMSIS/Include
        ../../Subsurface/Node/Common
        )

target_compile_options(${EXECUTABLE} PRIVATE
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard

        -fdata-sections
        -ffunction-sections

        -Wall

        -fno-exceptions
        $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>

        $<$<CONFIG:Debug>:-Og>
        )

target_link_options(${EXECUTABLE} PRIVATE
        -T${CMAKE_SOURCE_DIR}/STM32F303RETx_FLASH.ld
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard
        -specs=nano.specs
        -lc
        -lm
        -lnosys
        -Wl,-Map=${PROJECT_NAME}.map,--cref
        -Wl,--gc-sections
        )

# Print executable size
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND arm-none-eabi-size ${EXECUTABLE})

# Create hex file
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
        COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin)
