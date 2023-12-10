# Firmware
add_executable(openvario_fw 
    main.cpp

    board/${TARGET_PLATFORM}/ov_board.cpp

    console/debug_console.cpp

    filesystem/fs.cpp
    filesystem/dir.cpp
    filesystem/file.cpp
    filesystem/fs_console.cpp
)

# Include directories
target_include_directories(openvario_fw PRIVATE 
    .
    board
    board/${TARGET_PLATFORM}
    console
    filesystem
)

# Linked libraries
target_link_libraries(openvario_fw PRIVATE
    littlefs
    openvario_peripherals
    $<TARGET_OBJECTS:openvario_os>
    $<TARGET_OBJECTS:openvario_bsp>
)

# Linker script
target_link_options(openvario_fw PRIVATE -T${CMAKE_CURRENT_LIST_DIR}/linker/${TARGET_PLATFORM}.ld -Wl,-Map=${BIN_DIR}/openvario_fw.map )
