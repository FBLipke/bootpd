cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE PLUGIN_SOURCES CONFIGURE_DEPENDS "*.cpp")

add_library(DHCPService SHARED ${PLUGIN_SOURCES})

target_include_directories(DHCPService PUBLIC 
    ${CMAKE_SOURCE_DIR}/bootpd/src
    ${CMAKE_SOURCE_DIR}/thirdparty
)

if(WIN32)
    target_link_libraries(DHCPService PRIVATE Ws2_32.lib)
endif()

# Output to plugins/ directory (same level as executable)
if(WIN32)
    set_target_properties(DHCPService PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
        PREFIX ""
    )
else()
    set_target_properties(DHCPService PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
        PREFIX ""
    )
endif()

set_target_properties(DHCPService PROPERTIES
    VERSION 1.0.0
    SOVERSION 1
)
