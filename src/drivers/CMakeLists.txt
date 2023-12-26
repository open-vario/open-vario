# OS library
file (GLOB DRIVERS_SOURCE_FILES "${TARGET_PLATFORM}/*.cpp")
add_library(openvario_drivers
    ${DRIVERS_SOURCE_FILES}

    spi_pin_cs_driver.cpp
)

# Include directories
target_include_directories(openvario_drivers PUBLIC 
    .
    ${TARGET_PLATFORM}
)

# Linked libraries
target_link_libraries(openvario_drivers PUBLIC
    openvario_os
)
