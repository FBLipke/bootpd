#pragma once
#include "IBootService.h"
#include <string>
#include <vector>
#include <filesystem>

namespace bootp::Plugins::DHCP::BootServices
{
	/**
	 * @brief BootServiceLoader - Loads BootService plugins from DLL files
	 * 
	 * Scans a directory for BootService DLLs and loads them.
	 * Each DLL must export: bool RegisterBootService()
	 * 
	 * Directory structure:
	 * plugins/
	 * └── DHCPService/
	 *     └── BootServices/
	 *         ├── PxeRBCP.dll
	 *         ├── MSRIS.dll
	 *         └── ...
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
		 * @brief Load a single BootService DLL
		 * @param dllPath Path to the DLL file
		 * @return true if loaded successfully
		 */
		static bool LoadDll(const std::string& dllPath);

		/**
		 * @brief Unload all loaded BootService DLLs
		 */
		static void UnloadAll();

		/**
		 * @brief Get the number of loaded DLLs
		 */
		static size_t GetLoadedCount();

	private:
		// Store loaded DLL handles (platform-specific)
		struct LoadedDll
		{
			void* handle = nullptr;
			std::string path;
		};

		static std::vector<LoadedDll>& GetLoadedDlls();
	};
}
