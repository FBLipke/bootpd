cmake_minimum_required(VERSION 3.10)
project(bootpd VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
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

# ===== Bootpd Network Library =====
set(NETWORK_SOURCES
${CMAKE_SOURCE_DIR}/bootpd/src/Bootpd/Network/Packet/Packet.cpp
${CMAKE_SOURCE_DIR}/bootpd/src/Bootpd/Network/ServerManager.cpp
${CMAKE_SOURCE_DIR}/bootpd/src/Bootpd/Network/ClientManager.cpp
${CMAKE_SOURCE_DIR}/bootpd/src/Bootpd/Network/Server/Server.cpp
${CMAKE_SOURCE_DIR}/bootpd/src/Bootpd/Network/Client/Client.cpp
${CMAKE_SOURCE_DIR}/bootpd/src/Bootpd/Network/Socket/Socket.cpp
)
add_library(bootpd_network STATIC ${NETWORK_SOURCES})
target_include_directories(bootpd_network PUBLIC
${CMAKE_SOURCE_DIR}/bootpd/src
)

if(WIN32)
target_link_libraries(bootpd_network PRIVATE Ws2_32.lib Rpcrt4.lib IPHLPAPI.lib)
else()
target_link_libraries(bootpd_network PRIVATE)
endif()

# ===== Bootpd Platform Library =====
set(PLATFORM_SOURCES
${CMAKE_SOURCE_DIR}/bootpd/src/Bootpd/Environment/Functions.cpp
${CMAKE_SOURCE_DIR}/bootpd/src/Bootpd/Environment/Filesystem.cpp
${CMAKE_SOURCE_DIR}/bootpd/src/Bootpd/Environment/Environment.cpp
)
add_library(bootpd_platform STATIC ${PLATFORM_SOURCES})
target_include_directories(bootpd_platform PUBLIC
${CMAKE_SOURCE_DIR}/bootpd/src
${CMAKE_SOURCE_DIR}/thirdparty
)
if(WIN32)
target_link_libraries(bootpd_platform PRIVATE)
else()
target_link_libraries(bootpd_platform PRIVATE)
endif()

# ===== Bootpd Database Library =====
set(DATABASE_SOURCES
${CMAKE_SOURCE_DIR}/bootpd/src/Bootpd/Database/DatabaseConfig.cpp
${CMAKE_SOURCE_DIR}/bootpd/src/Bootpd/Database/SQLiteDatabase.cpp
)
add_library(bootpd_database STATIC ${DATABASE_SOURCES})
add_dependencies(bootpd_database bootpd_platform sqlite3_lib)
target_include_directories(bootpd_database PUBLIC
${CMAKE_SOURCE_DIR}/bootpd/src
${CMAKE_SOURCE_DIR}/thirdparty
)
if(WIN32)
target_link_libraries(bootpd_database PRIVATE bootpd_platform sqlite3_lib)
else()
target_link_libraries(bootpd_database PRIVATE bootpd_platform sqlite3_lib)
endif()

# ===== Plugin API Library (standalone, no external dependencies) =====
add_subdirectory(libs/bootpd_plugin_api)

# ===== Bootpd Common Library (Functions, Filesystem, Database, etc.) =====
set(COMMON_SOURCES

${CMAKE_SOURCE_DIR}/bootpd/src/Bootpd/Bootpd.cpp
${CMAKE_SOURCE_DIR}/bootpd/src/Bootpd/Services/ServiceManager.cpp

)
add_library(bootpd_common SHARED ${COMMON_SOURCES})
target_include_directories(bootpd_common PUBLIC
${CMAKE_SOURCE_DIR}/bootpd/src
${CMAKE_SOURCE_DIR}/thirdparty
${CMAKE_SOURCE_DIR}/libs/bootpd_plugin_api/include
)
if(WIN32)
target_link_libraries(bootpd_common PRIVATE bootpd_platform bootpd_database bootpd_network bootpd_plugin_api tinyxml2_lib )
else()
target_link_libraries(bootpd_common PRIVATE bootpd_platform bootpd_database bootpd_network bootpd_plugin_api tinyxml2_lib pthread)
endif()

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
set_target_properties(bootpd PROPERTIES ENABLE_EXPORTS ON LINK_FLAGS "-rdynamic")

target_include_directories(bootpd PRIVATE
${CMAKE_SOURCE_DIR}/bootpd/src
${CMAKE_SOURCE_DIR}/thirdparty
)
if(WIN32)
target_link_libraries(bootpd PRIVATE bootpd_network bootpd_common bootpd_platform bootpd_database tinyxml2_lib)
else()
target_link_libraries(bootpd PRIVATE bootpd_network bootpd_common bootpd_platform bootpd_database pthread tinyxml2_lib)
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
