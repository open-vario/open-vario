function(find_compiler compiler_path compiler_ext compiler_exe)
    find_program(
        _compiler ${compiler_exe}
        PATHS ${CMAKE_TOOLCHAIN_PATH} PATH_SUFFIXES bin
    )

    if("${_compiler}" STREQUAL "${_compiler}-NOTFOUND")
        message(FATAL_ERROR "Compiler not found, you can specify search path with \"CMAKE_TOOLCHAIN_PATH\".")
    else()
        message(STATUS "Found compiler ${_compiler}")
        get_filename_component(COMPILER_PATH ${_compiler} DIRECTORY CACHE)
        get_filename_component(COMPILER_EXT ${_compiler} EXT CACHE)

        set(${compiler_path} ${COMPILER_PATH} PARENT_SCOPE)
        set(${compiler_ext} ${COMPILER_EXT} PARENT_SCOPE)
    endif()
endfunction()
