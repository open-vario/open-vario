# OS library
file (GLOB DRIVERS_SOURCE_FILES "${TARGET_PLATFORM}/*.cpp")
add_library(openvario_drivers
    ${DRIVERS_SOURCE_FILES}

    generic/soft_i2c.cpp
    generic/spi_pin_cs_driver.cpp
)

# Include directories
target_include_directories(openvario_drivers PUBLIC 
    .
    generic
    ${TARGET_PLATFORM}
)

# Linked libraries
target_link_libraries(openvario_drivers PUBLIC
    openvario_os
)
