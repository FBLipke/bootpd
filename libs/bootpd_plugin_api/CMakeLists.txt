cmake_minimum_required(VERSION 3.15)
project(bootpd_plugin_api)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files for the plugin API library
set(PLUGIN_API_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BootServiceRegistry.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BootServiceLoader.cpp
)

# Create the static library (for compile-time linking)
add_library(${PROJECT_NAME} STATIC ${PLUGIN_API_SOURCES})

# Include directories (public for plugins)
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Compiler flags
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /utf-8>
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
)

# Install targets
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION include/bootp
)
