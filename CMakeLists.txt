cmake_minimum_required(VERSION 3.10)
project(bootpd VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Platform detection
if(WIN32)
    add_definitions(-D_WIN32=1)
elseif(UNIX AND NOT APPLE)
    add_definitions(-D_LINUX=1)
endif()

# Source files
set(SOURCES
    bootpd/src/main.cpp
    bootpd/src/Bootpd/Bootpd.cpp
    bootpd/src/Bootpd/Common/Functions.cpp
    bootpd/src/Bootpd/Network/Server.cpp
    bootpd/src/Bootpd/Network/ServerManager.cpp
    bootpd/src/Bootpd/Network/Socket.cpp
)

# Headers
set(HEADERS
    bootpd/src/Bootpd/Bootpd.h
    bootpd/src/Bootpd/Common/Functions.h
    bootpd/src/Bootpd/Network/IServer.h
    bootpd/src/Bootpd/Network/ISocket.h
    bootpd/src/Bootpd/Network/Server.h
    bootpd/src/Bootpd/Network/ServerManager.h
    bootpd/src/Bootpd/Network/Socket.h
)

add_executable(bootpd ${SOURCES} ${HEADERS})

# Platform-specific libraries
if(WIN32)
    target_link_libraries(bootpd Ws2_32.lib Rpcrt4.lib)
elseif(UNIX AND NOT APPLE)
    target_link_libraries(bootpd pthread)
endif()

# Warnings
if(MSVC)
    target_compile_options(bootpd PRIVATE /W4 /WX-)
else()
    target_compile_options(bootpd PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Install
install(TARGETS bootpd DESTINATION bin)
