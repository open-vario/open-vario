
# FreeRTOS
set(FREERTOS_HEAP "${CMAKE_SOURCE_DIR}/src/firmware/freertos/dummy_heap.c")
add_library(freertos_config INTERFACE)
target_include_directories(freertos_config INTERFACE ${CMAKE_SOURCE_DIR}/src/firmware/freertos)
add_subdirectory(freertos-10.6.2)

# littlefs
add_library(littlefs STATIC 
    ${CMAKE_CURRENT_LIST_DIR}/littlefs-2.8.1/lfs.c 
    ${CMAKE_CURRENT_LIST_DIR}/littlefs-2.8.1/lfs_util.c
)
target_include_directories(littlefs PUBLIC ${CMAKE_CURRENT_LIST_DIR}/littlefs-2.8.1)
target_compile_options(littlefs PUBLIC -Wno-shadow)
