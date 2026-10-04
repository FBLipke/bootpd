# plugins/CMakeLists.txt
# Dynamically discover and build all plugins

# Banner


# Find all plugin directories (each must have a CMakeLists.txt)
file(GLOB PLUGIN_SUBDIRS "${CMAKE_CURRENT_SOURCE_DIR}/*")

# Build each plugin directory
foreach(dir ${PLUGIN_SUBDIRS})
    if(IS_DIRECTORY ${dir})
        # Get plugin name from directory
        get_filename_component(plugin_name ${dir} NAME)

        # Skip non-plugin directories
        if(plugin_name STREQUAL "CMakeLists.txt" OR plugin_name STREQUAL "plugins")
            continue()
        endif()

        # Skip NBP on non-Windows (freestanding stdlib, for PXE boot only)
        if(plugin_name STREQUAL "nbp" AND NOT WIN32)
            message(WARNING "Skipping NBP plugin on non-Windows platform")
            continue()
        endif()

        message(STATUS "Found plugin: ${plugin_name}")

        # Check if plugin has CMakeLists.txt
        if(EXISTS ${dir}/CMakeLists.txt)
            message(STATUS "Building plugin: ${plugin_name}")
            add_subdirectory(${dir})
        else()
            message(WARNING "Plugin ${plugin_name} has no CMakeLists.txt, skipping")
        endif()
    endif()
endforeach()
