# Compiler flags needed to compile for this CPU
set(CPU_FLAGS "--cpu Cortex-M7 --fpu VFPv5_sp")

include(${CMAKE_CURRENT_LIST_DIR}/arm-cortex-iar-toolchain.cmake)
