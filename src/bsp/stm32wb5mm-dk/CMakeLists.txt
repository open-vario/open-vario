
# BSP library
file (GLOB STM32WBxx_HAL_SOURCE_FILES "STM32WBxx_HAL_Driver/Src/*.c")
add_library(openvario_bsp STATIC
    ${STM32WBxx_HAL_SOURCE_FILES}

    STM32_WPAN/ble/core/auto/ble_gap_aci.c
    STM32_WPAN/ble/core/auto/ble_gatt_aci.c
    STM32_WPAN/ble/core/auto/ble_hal_aci.c
    STM32_WPAN/ble/core/auto/ble_hci_le.c
    STM32_WPAN/ble/core/auto/ble_l2cap_aci.c
    STM32_WPAN/ble/core/template/osal.c
    STM32_WPAN/ble/svc/Src/dis.c
    STM32_WPAN/ble/svc/Src/hrs.c
    STM32_WPAN/ble/svc/Src/svc_ctl.c
    STM32_WPAN/interface/patterns/ble_thread/shci/shci.c
    STM32_WPAN/interface/patterns/ble_thread/tl/hci_tl.c
    STM32_WPAN/interface/patterns/ble_thread/tl/hci_tl_if.c
    STM32_WPAN/interface/patterns/ble_thread/tl/shci_tl.c
    STM32_WPAN/interface/patterns/ble_thread/tl/shci_tl_if.c
    STM32_WPAN/interface/patterns/ble_thread/tl/tl_mbox.c
    STM32_WPAN/utilities/dbg_trace.c
    STM32_WPAN/utilities/stm_list.c
    STM32_WPAN/utilities/stm_queue.c
    STM32_WPAN/utilities/otp.c

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

    STM32_WPAN
    STM32_WPAN/ble
    STM32_WPAN/ble/core
    STM32_WPAN/ble/core/auto
    STM32_WPAN/ble/core/template
    STM32_WPAN/ble/svc/Inc
    STM32_WPAN/interface/patterns/ble_thread
    STM32_WPAN/interface/patterns/ble_thread/shci
    STM32_WPAN/interface/patterns/ble_thread/tl
    STM32_WPAN/utilities
)
