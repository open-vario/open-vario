# Peripherals library
add_library(openvario_peripherals
    barometric_altimeter.cpp
    ms56xx.cpp
    ms56xx_i2c.cpp
    i_gnss.cpp
    nmea_gnss.cpp
    pin_button.cpp
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
