cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE PLUGIN_SOURCES CONFIGURE_DEPENDS "*.cpp")

add_library(DHCPService SHARED ${PLUGIN_SOURCES})

target_include_directories(DHCPService PUBLIC
${CMAKE_SOURCE_DIR}/bootpd/src
${CMAKE_SOURCE_DIR}/thirdparty
)

add_dependencies(DHCPService PacketLib tinyxml2_lib)

if(WIN32)
target_link_libraries(DHCPService PRIVATE Ws2_32.lib PacketLib bootpd_common Rpcrt4.lib IPHLPAPI.lib tinyxml2_lib)
else()
target_link_libraries(DHCPService PRIVATE PacketLib bootpd_common tinyxml2_lib)
endif()

set_target_properties(DHCPService PROPERTIES
LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/Debug/plugins/"
RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/Debug/plugins/"
PREFIX ""
)

set_target_properties(DHCPService PROPERTIES
VERSION 1.0.0
SOVERSION 1
)
