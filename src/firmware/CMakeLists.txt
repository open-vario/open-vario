# Firmware
add_executable(openvario_fw 
    main.cpp

    app/ov_app.cpp

    board/${TARGET_PLATFORM}/ov_board.cpp

    config/ov_config.cpp
    config/config_console.cpp

    console/debug_console.cpp

    filesystem/fs.cpp
    filesystem/dir.cpp
    filesystem/file.cpp
    filesystem/fs_console.cpp

    hmi/hmi_console.cpp
    hmi/hmi_manager.cpp
    hmi/screens/base_screen.cpp
    hmi/screens/dashboard1_screen.cpp
    hmi/screens/dashboard2_screen.cpp
    hmi/screens/dashboard3_screen.cpp
    hmi/screens/settings_screen.cpp
    hmi/screens/settings_display_screen.cpp
    hmi/screens/settings_exit_screen.cpp
    hmi/screens/splash_screen.cpp
)

# Include directories
target_include_directories(openvario_fw PRIVATE 
    .
    app
    board
    board/${TARGET_PLATFORM}
    config
    console
    filesystem
    hmi
    hmi/screens
)

# Linked libraries
target_link_libraries(openvario_fw PRIVATE
    littlefs
    yacswl
    openvario_peripherals
    $<TARGET_OBJECTS:openvario_os>
    $<TARGET_OBJECTS:openvario_bsp>
)

# Linker script
target_link_options(openvario_fw PRIVATE -T${CMAKE_CURRENT_LIST_DIR}/linker/${TARGET_PLATFORM}.ld -Wl,-Map=${BIN_DIR}/openvario_fw.map )
