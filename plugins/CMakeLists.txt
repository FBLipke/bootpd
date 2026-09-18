# plugins/CMakeLists.txt
# Dynamically discover and build all plugins

# Banner
message("")
message(STATUS "========================================")
message(STATUS "         B O O T P D   P L U G I N S")
message(STATUS "========================================")
message("")

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
