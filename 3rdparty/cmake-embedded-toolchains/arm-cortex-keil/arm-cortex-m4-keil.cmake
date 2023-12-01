# Compiler flags needed to compile for this CPU
#set(CMAKE_SYSTEM_PROCESSOR cortex-m4)
#set(CMAKE_SYSTEM_ARCH armv7e-m)
set(CPU_FLAGS "--target=arm-arm-none-eabi -mthumb -mcpu=cortex-m4 -march=armv7e-m")
set(VFP_FLAGS "-mfloat-abi=hard -mfpu=fpv4-sp-d16")
set(LINK_FLAGS "--cpu=Cortex-M4.fp.sp")

include(${CMAKE_CURRENT_LIST_DIR}/arm-cortex-keil-toolchain.cmake)
