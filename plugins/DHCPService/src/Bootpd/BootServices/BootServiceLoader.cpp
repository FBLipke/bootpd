#include "BootServiceLoader.h"
#include <iostream>
#include <windows.h>

namespace bootp::Plugins::DHCP::BootServices
{
	typedef bool(*RegisterBootServiceFn)();

	static std::vector<HMODULE>& GetLoadedModules()
	{
		static std::vector<HMODULE> modules;
		return modules;
	}

	size_t BootServiceLoader::LoadFromDirectory(const std::string& path)
	{
		size_t loadedCount = 0;

		WIN32_FIND_DATAA findData;
		HANDLE hFind = FindFirstFileA((path + "\\*.dll").c_str(), &findData);

		if (hFind == INVALID_HANDLE_VALUE)
		{
			return 0;
		}

		do
		{
			std::string dllPath = path + "\\" + std::string(findData.cFileName);
			
			HMODULE handle = LoadLibraryA(dllPath.c_str());
			if (!handle)
			{
				std::cerr << "Failed to load DLL: " << dllPath 
						  << " (Error: " << GetLastError() << ")" << std::endl;
				continue;
			}

			auto registerFn = (RegisterBootServiceFn)GetProcAddress(handle, "RegisterBootService");
			if (!registerFn)
			{
				std::cerr << "Failed to find RegisterBootService in: " << dllPath << std::endl;
				FreeLibrary(handle);
				continue;
			}

			if (!registerFn())
			{
				std::cerr << "RegisterBootService failed in: " << dllPath << std::endl;
				FreeLibrary(handle);
				continue;
			}

			GetLoadedModules().push_back(handle);
			loadedCount++;

		} while (FindNextFileA(hFind, &findData));

		FindClose(hFind);
		return loadedCount;
	}

	void BootServiceLoader::UnloadAll()
	{
		for (HMODULE handle : GetLoadedModules())
		{
			if (handle)
			{
				FreeLibrary(handle);
			}
		}
		GetLoadedModules().clear();
	}

	size_t BootServiceLoader::GetLoadedCount()
	{
		return GetLoadedModules().size();
	}
}
