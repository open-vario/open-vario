include("${CMAKE_CURRENT_LIST_DIR}/../find_compiler.cmake")

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR rx)
set(TARGET_TRIPLET "rx-elf-")

# Find the compiler path
find_compiler(RX_COMPILER_PATH RX_COMPILER_EXT "iccarm.exe")

# perform compiler test with the static library
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_COMPILER    ${RX_COMPILER_PATH}/${TARGET_TRIPLET}gcc${RX_COMPILER_EXT})
set(CMAKE_CXX_COMPILER  ${RX_COMPILER_PATH}/${TARGET_TRIPLET}g++${RX_COMPILER_EXT})
set(CMAKE_ASM_COMPILER  ${RX_COMPILER_PATH}/${TARGET_TRIPLET}gcc${RX_COMPILER_EXT})
set(CMAKE_LINKER        ${RX_COMPILER_PATH}/${TARGET_TRIPLET}gcc${RX_COMPILER_EXT})
set(CMAKE_SIZE_UTIL     ${RX_COMPILER_PATH}/${TARGET_TRIPLET}size${RX_COMPILER_EXT})
set(CMAKE_OBJCOPY       ${RX_COMPILER_PATH}/${TARGET_TRIPLET}objcopy${RX_COMPILER_EXT})
set(CMAKE_OBJDUMP       ${RX_COMPILER_PATH}/${TARGET_TRIPLET}objdump${RX_COMPILER_EXT})
set(CMAKE_NM_UTIL       ${RX_COMPILER_PATH}/${TARGET_TRIPLET}nm${RX_COMPILER_EXT})

set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(CMAKE_COMMON_FLAGS "-ffunction-sections -fdata-sections -fdiagnostics-parseable-fixits -fno-strict-aliasing -fno-builtin -fshort-enums -Wuninitialized -Wdouble-promotion -Werror -Wno-unused-function -Wno-unused-parameter -Wno-incompatible-pointer-types")
set(CMAKE_C_FLAGS_INIT 	"${MCPU_FLAGS} ${VFP_FLAGS} ${CMAKE_COMMON_FLAGS} ${LIBC_INCLUDE}")
set(CMAKE_CXX_FLAGS_INIT "${MCPU_FLAGS} ${VFP_FLAGS} ${CMAKE_COMMON_FLAGS}")
set(CMAKE_ASM_FLAGS_INIT "${MCPU_FLAGS} ${VFP_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_INIT "${LD_FLAGS} -fno-common -nostartfiles -Wl,--gc-sections --specs=nano.specs")

set(CMAKE_C_FLAGS_DEBUG_INIT "-O0 -g3")
set(CMAKE_CXX_ASM_FLAGS_DEBUG_INIT "-O0 -g3")
set(CMAKE_ASM_FLAGS_DEBUG_INIT "-g3")

set(CMAKE_C_FLAGS_RELEASE_INIT "-Os")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Os")
set(CMAKE_ASM_FLAGS_RELEASE_INIT "")
