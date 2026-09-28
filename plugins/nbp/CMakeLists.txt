# NBP Plugin - Network Boot Program
# Builds PXE NBP using stdlib

cmake_minimum_required(VERSION 3.10)
project(nbp C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Output directory
set(NBP_OUTPUT_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/nbp)
file(MAKE_DIRECTORY ${NBP_OUTPUT_DIR})

# Source files
file(GLOB NBP_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.S
)

# Stdlib sources
file(GLOB NBP_STDLIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/stdlib/*.c
)

# Combine all sources
set(ALL_NBP_SOURCES ${NBP_SOURCES} ${NBP_STDLIB_SOURCES})

# Include directories
set(NBP_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/stdlib/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Create executable
add_executable(nbp ${ALL_NBP_SOURCES})

target_include_directories(nbp PRIVATE ${NBP_INCLUDE_DIRS})

# Customizations for 16-bit real mode code
if(CMAKE_C_COMPILER_ID MATCHES "GNU")
    # 16-bit code needs specific flags for real mode
    target_compile_options(nbp PRIVATE
        -m32
        -fno-pie
    )
    
    # Linker script for 16-bit output
    target_link_libraries(nbp -T ${CMAKE_CURRENT_SOURCE_DIR}/src/linker.ld -melf_i386)
elseif(MSVC)
    target_compile_options(nbp PRIVATE /safeseh)
endif()

# Copy output to bin folder
add_custom_command(TARGET nbp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:nbp>
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/nbp.rom
)

set_target_properties(nbp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${NBP_OUTPUT_DIR}
)
