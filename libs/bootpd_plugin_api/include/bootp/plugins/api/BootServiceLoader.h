#pragma once
#include "Types.h"
#include "IBootService.h"
#include <string>

namespace bootp::plugins::api
{
	/**
	 * @brief BootServiceLoader - Loads BootService plugins from DLL/SO files
	 * 
	 * Cross-platform plugin loader.
	 * Each plugin DLL must export: bool RegisterBootService()
	 */
	class BootServiceLoader
	{
	public:
		/**
		 * @brief Load all BootService plugins from a directory
		 * @param path Directory containing plugin files (*.dll on Windows, *.so on Linux)
		 * @return Number of services loaded successfully
		 */
		static size LoadFromDirectory(const std::string& path);

		/**
		 * @brief Unload all loaded plugin DLLs
		 */
		static void UnloadAll();

		/**
		 * @brief Get the number of currently loaded plugin DLLs
		 */
		static size GetLoadedCount();

		/**
		 * @brief Load a single plugin from a specific path
		 * @param path Full path to the plugin DLL
		 * @return true if loaded successfully
		 */
		static bool LoadPlugin(const std::string& path);
	};
}
