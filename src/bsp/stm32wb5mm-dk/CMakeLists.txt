
# BSP library
file (GLOB STM32WBxx_HAL_SOURCE_FILES "STM32WBxx_HAL_Driver/Src/*.c")
add_library(openvario_bsp STATIC
    ${STM32WBxx_HAL_SOURCE_FILES}

    startup/startup_stm32wb5mmghx.s
    startup/system_stm32wbxx.c
    startup/syscalls.c
)

# Exported includes
target_include_directories(openvario_bsp PUBLIC
    conf

    CMSIS/Include
    CMSIS/Device/ST/STM32WBxx/Include

    STM32WBxx_HAL_Driver/Inc
)
