#################################################################################
#                               Open Vario                                      #
#################################################################################

cmake_minimum_required(VERSION 3.18)

project(OpenVario DESCRIPTION "Open source variometer for paragliding and hang gliding"
                  VERSION 1.0.0
)

# Definitions for Version.h file
add_definitions(-DOPENVARIO_MAJOR="${PROJECT_VERSION_MAJOR}")
add_definitions(-DOPENVARIO_MINOR="${PROJECT_VERSION_MINOR}")
add_definitions(-DOPENVARIO_FIX="${PROJECT_VERSION_PATCH}")

# Build options
include(${CMAKE_CURRENT_LIST_DIR}/cmake/CMakeLists_Options.txt)

# Target specific definitions
if(NOT DEFINED TARGET_PLATFORM)
    message(FATAL_ERROR "TARGET_PLATFORM must be defined")
endif()

# Default output directories
if(NOT DEFINED BIN_DIR)
    message(BIN_DIR "BIN_DIR must be defined")
endif()
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

# C/C++ and assembly standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
enable_language(ASM)

# 3rd party libraries
add_subdirectory(3rdparty)

# Open Vario sources
add_subdirectory(src)
