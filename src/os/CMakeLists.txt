# OS library
add_library(openvario_os
    callbacks.c
    mutex.cpp
    os.cpp
    semaphore.cpp
    thread.cpp
    timer.cpp
)

# Include directories
target_include_directories(openvario_os PUBLIC 
    .
)

# Linked libraries
target_link_libraries(openvario_os PUBLIC
    freertos_kernel
    freertos_kernel_port
    openvario_bsp
)
