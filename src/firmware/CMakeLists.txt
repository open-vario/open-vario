# Firmware
add_executable(openvario_fw 
    main.cpp

    freertos/callbacks.c
    freertos/mutex.cpp
    freertos/os.cpp
    freertos/semaphore.cpp
    freertos/thread.cpp
)

# Include directories
target_include_directories(openvario_fw PRIVATE 
    .
    freertos
)

# Linked libraries
target_link_libraries(openvario_fw PRIVATE
    freertos_kernel
    freertos_kernel_port
    openvario_bsp
)

# Linker script
target_link_options(openvario_fw PRIVATE -T${CMAKE_CURRENT_LIST_DIR}/linker/${TARGET_PLATFORM}.ld -Wl,-Map=${BIN_DIR}/openvario_fw.map)
