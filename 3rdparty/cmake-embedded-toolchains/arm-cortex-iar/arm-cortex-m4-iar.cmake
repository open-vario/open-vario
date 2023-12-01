# Compiler flags needed to compile for this CPU
set(CPU_FLAGS "--cpu Cortex-M4 --fpu VFPv4_sp")

include(${CMAKE_CURRENT_LIST_DIR}/arm-cortex-iar-toolchain.cmake)
