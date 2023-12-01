function(target_link TARGET LINKER_SCRIPT)
    set_target_properties(${TARGET} PROPERTIES SUFFIX ".elf")

    if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
        target_link_options(${TARGET} 
            PRIVATE 
                -T${LINKER_SCRIPT}.ld
                -Wl,-Map=${TARGET}.map
        )
        add_custom_target(${TARGET}.bin ALL
            DEPENDS ${TARGET}
            COMMAND ${CMAKE_OBJCOPY} -Obinary ${TARGET}.elf ${TARGET}.bin
            COMMAND ${CMAKE_OBJCOPY} -Oihex ${TARGET}.elf ${TARGET}.hex
        )

    elseif(CMAKE_C_COMPILER_ID STREQUAL "IAR")
        target_link_options(${TARGET} 
            PRIVATE 
                --config ${LINKER_SCRIPT}.icf
                --map=${TARGET}.map
        )
        add_custom_target(${TARGET}.bin ALL
            DEPENDS ${TARGET}
            COMMAND ${CMAKE_IAR_ELFTOOL} --bin ${TARGET}.elf ${TARGET}.bin
        )

    elseif(CMAKE_C_COMPILER_ID STREQUAL "ARMClang")
        target_link_options(${TARGET} 
            PRIVATE 
                --scatter ${LINKER_SCRIPT}.sct
                --map
        )
        add_custom_target(${TARGET}.bin ALL
            DEPENDS ${TARGET}
            COMMAND ${CMAKE_FROMELF} --bin --output=${TARGET}.bin ${TARGET}.elf
            COMMAND ${CMAKE_FROMELF} --i32combined --output=${TARGET}.hex ${TARGET}.elf
        )
    else()
        message(FATAL_ERROR "Unknown CMAKE_C_COMPILER_ID ${CMAKE_C_COMPILER_ID}")
    endif()
endfunction()
