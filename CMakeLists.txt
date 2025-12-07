cmake_minimum_required(VERSION 3.20)
project(bomberman C ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Toolchain configuration
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv32)
set(TOOLCHAIN_PREFIX riscv32-unknown-elf-)

set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}objcopy)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_ASM_COMPILER_WORKS 1)

# Use ld directly
set(CMAKE_C_LINK_EXECUTABLE "${TOOLCHAIN_PREFIX}ld <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
set(CMAKE_C_LINK_DEPENDS_NO_SHARED TRUE)
set(CMAKE_LINK_DEPENDS_NO_SHARED TRUE)

# Compiler flags
add_compile_options(-Wall -nostdlib -O3 -mabi=ilp32 -march=rv32imzicsr -fno-builtin -g)

# Find sources
file(GLOB_RECURSE SOURCES src/**.c src/*.S lib/*.c lib/*.S)

# Build executable
add_executable(bomberman ${SOURCES})
set_target_properties(bomberman PROPERTIES 
    SUFFIX ".elf"
    LINK_DEPENDS_NO_SHARED TRUE
)

target_link_options(bomberman PRIVATE -T${CMAKE_SOURCE_DIR}/lib/dtekv-script.lds)
target_link_libraries(bomberman PRIVATE ${CMAKE_SOURCE_DIR}/lib/softfloat.a)
target_include_directories(bomberman PRIVATE 
    ${CMAKE_SOURCE_DIR}/includes
    ${CMAKE_SOURCE_DIR}/lib
)


# Generate .bin and disassembly
add_custom_command(TARGET bomberman POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary bomberman.elf bomberman.bin
)
