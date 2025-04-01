set(CMAKE_SYSTEM_NAME Generic)

find_program(CLANG clang REQUIRED True)
set(CMAKE_C_COMPILER clang)
set(CMAKE_ASM_COMPILER clang)

find_program(CLANGXX clang++ REQUIRED False)
set(CMAKE_CXX_COMPILER ${CLANGXX})

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_C_COMPILER_TARGET x86_64-none-elf)
set(CMAKE_CXX_COMPILER_TARGET x86_64-none-elf)
add_compile_options(-target ${CMAKE_C_COMPILER_TARGET})
add_link_options(-target ${CMAKE_C_COMPILER_TARGET})

set(ARCH_SOURCES
    src/arch/x86_64/syscall.S)
