# Peripherals library
add_library(openvario_peripherals
    s25flxxxs.cpp
    ssd1315.cpp
)

# Include directories
target_include_directories(openvario_peripherals PUBLIC 
    .
)

# Linked libraries
target_link_libraries(openvario_peripherals PUBLIC
    openvario_drivers
)
