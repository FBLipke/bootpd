cmake_minimum_required(VERSION 3.10)
project(bootpd VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ===== SQLite static library from thirdparty =====
set(SQLITE_SRC_DIR ${CMAKE_SOURCE_DIR}/thirdparty/sqlite3)
add_library(sqlite3_lib STATIC ${SQLITE_SRC_DIR}/sqlite3.c)
set_source_files_properties(${SQLITE_SRC_DIR}/sqlite3.c PROPERTIES LANGUAGE C)

# ===== Packet Static Library (used by main + plugins) =====
set(PACKET_SOURCES
${CMAKE_SOURCE_DIR}/bootpd/src/Bootpd/Network/Packet/Packet.cpp
)
add_library(PacketLib STATIC ${PACKET_SOURCES})
target_include_directories(PacketLib PUBLIC
${CMAKE_SOURCE_DIR}/bootpd/src
)


# ===== Source Files =====
file(GLOB_RECURSE ALL_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/bootpd/src/*.cpp")

# Exclude plugins from main build
file(GLOB_RECURSE PLUGIN_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/plugins/*.cpp")
foreach(plugin_src ${PLUGIN_SOURCES})
list(FIND ALL_SOURCES ${plugin_src} _index)
if(NOT ${_index} EQUAL -1)
list(REMOVE_ITEM ALL_SOURCES ${plugin_src})
endif()
endforeach()

# Remove Packet.cpp from main build (it's in PacketLib now)
list(FIND ALL_SOURCES "${CMAKE_SOURCE_DIR}/bootpd/src/Bootpd/Network/Packet/Packet.cpp" _idx)
if(NOT ${_idx} EQUAL -1)
list(REMOVE_ITEM ALL_SOURCES "${CMAKE_SOURCE_DIR}/bootpd/src/Bootpd/Network/Packet/Packet.cpp")
endif()
add_library(tinyxml2_lib STATIC ${CMAKE_SOURCE_DIR}/thirdparty/tinyxml2/tinyxml2.cpp)

add_executable(bootpd ${ALL_SOURCES})
set_target_properties(bootpd PROPERTIES LINK_FLAGS "-rdynamic")

target_include_directories(bootpd PRIVATE
${CMAKE_SOURCE_DIR}/bootpd/src
${CMAKE_SOURCE_DIR}/thirdparty
)
if(WIN32)
target_link_libraries(bootpd PRIVATE PacketLib sqlite3_lib Ws2_32.lib Rpcrt4.lib IPHLPAPI.lib tinyxml2_lib)
else()
target_link_libraries(bootpd PRIVATE PacketLib sqlite3_lib pthread tinyxml2_lib)
endif()

# Build plugins as subdirectory
add_subdirectory(plugins)
# Erstelle Verzeichnis
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Config)

# Dann kopiere
configure_file(
${CMAKE_SOURCE_DIR}/Config/config.xml
${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Config/config.xml
)
