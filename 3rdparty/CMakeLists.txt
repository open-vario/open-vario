
# FreeRTOS
set(FREERTOS_HEAP "${CMAKE_SOURCE_DIR}/src/firmware/freertos/dummy_heap.c")
add_library(freertos_config INTERFACE)
target_include_directories(freertos_config INTERFACE ${CMAKE_SOURCE_DIR}/src/firmware/freertos)
add_subdirectory(freertos-10.6.2)
