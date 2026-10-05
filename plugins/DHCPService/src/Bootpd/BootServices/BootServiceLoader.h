#pragma once
#include "IBootService.h"
#include <string>

namespace bootp::Plugins::DHCP::BootServices
{
	/**
	 * @brief BootServiceLoader - Loads BootService plugins from DLL files
	 * 
	 * Scans a directory for BootService DLLs and loads them.
	 * Each DLL must export: bool RegisterBootService()
	 */
	class BootServiceLoader
	{
	public:
		/**
		 * @brief Load all BootService DLLs from a directory
		 * @param path Directory containing BootService DLLs
		 * @return Number of services loaded
		 */
		static size_t LoadFromDirectory(const std::string& path);

		/**
		 * @brief Unload all loaded BootService DLLs
		 */
		static void UnloadAll();

		/**
		 * @brief Get the number of loaded DLLs
		 */
		static size_t GetLoadedCount();
	};
}
