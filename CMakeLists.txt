cmake_minimum_required(VERSION 3.10)
project(bootpd VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Automatically find .cpp files EXCEPT plugins (plugins are built separately)
file(GLOB_RECURSE ALL_SOURCES 
CONFIGURE_DEPENDS
"${CMAKE_SOURCE_DIR}/bootpd/src/*.cpp"
)

# Exclude plugins from main build
file(GLOB_RECURSE PLUGIN_SOURCES 
CONFIGURE_DEPENDS
"${CMAKE_SOURCE_DIR}/plugins/*.cpp"
)

# Remove plugin sources from main build
foreach(plugin_src ${PLUGIN_SOURCES})
    # CMake 3.10 compatible - manually filter
    list(FIND ALL_SOURCES ${plugin_src} _index)
    if(NOT ${_index} EQUAL -1)
        list(REMOVE_ITEM ALL_SOURCES ${plugin_src})
    endif()
endforeach()

add_executable(bootpd ${ALL_SOURCES})

target_include_directories(bootpd PRIVATE 
${CMAKE_SOURCE_DIR}/bootpd/src
${CMAKE_SOURCE_DIR}/thirdparty
)

if(WIN32)
target_link_libraries(bootpd Ws2_32.lib Rpcrt4.lib IPHLPAPI.lib)
else()
target_link_libraries(bootpd pthread)
endif()

# Build plugins as subdirectory
add_subdirectory(plugins)
