include("${CMAKE_CURRENT_LIST_DIR}/../find_compiler.cmake")

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Find the compiler path
find_compiler(EWARM_COMPILER_PATH EWARM_COMPILER_EXT "iccarm.exe")

# Set up the CMake variables for compiler and assembler
set(CMAKE_C_COMPILER "${EWARM_COMPILER_PATH}/iccarm.exe")
set(CMAKE_CXX_COMPILER "${EWARM_COMPILER_PATH}/iccarm.exe")
set(CMAKE_ASM_COMPILER "${EWARM_COMPILER_PATH}/iasmarm.exe")

set(CMAKE_C_FLAGS_INIT 	"${CPU_FLAGS} -e")
set(CMAKE_CXX_FLAGS_INIT "${CPU_FLAGS} --c++")
set(CMAKE_ASM_FLAGS_INIT "${CPU_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_INIT "--semihosting")

set(CMAKE_C_FLAGS_DEBUG_INIT "-On")
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-On")
