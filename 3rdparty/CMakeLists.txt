
# FreeRTOS
set(FREERTOS_HEAP "${CMAKE_SOURCE_DIR}/src/os/dummy_heap.c")
add_library(freertos_config INTERFACE)
target_include_directories(freertos_config INTERFACE ${CMAKE_SOURCE_DIR}/src/os)
add_subdirectory(freertos-10.6.2)

# littlefs
add_library(littlefs STATIC 
    ${CMAKE_CURRENT_LIST_DIR}/littlefs-2.8.1/lfs.c 
    ${CMAKE_CURRENT_LIST_DIR}/littlefs-2.8.1/lfs_util.c
)
target_include_directories(littlefs PUBLIC ${CMAKE_CURRENT_LIST_DIR}/littlefs-2.8.1)
target_compile_options(littlefs PUBLIC -Wno-shadow -DLFS_NO_MALLOC -DLFS_NO_ASSERT -DLFS_THREADSAFE -DLFS_NO_TRACE -DLFS_NO_DEBUG -DLFS_NO_WARN -DLFS_NO_ERROR)
