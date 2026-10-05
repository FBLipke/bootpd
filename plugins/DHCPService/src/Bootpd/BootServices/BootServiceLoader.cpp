#include "BootServiceLoader.h"
#include <iostream>

#ifdef _WIN32
    #include <windows.h>
    #define DLLEXPORT __declspec(dllexport)
    typedef HMODULE DLL_HANDLE;
    #define LOAD_LIBRARY(path) LoadLibraryA(path)
    #define FREE_LIBRARY(handle) FreeLibrary(handle)
    #define GET_PROC_ADDRESS(handle, name) GetProcAddress(handle, name)
#else
    #include <dlfcn.h>
    #define DLLEXPORT
    typedef void* DLL_HANDLE;
    #define LOAD_LIBRARY(path) dlopen(path, RTLD_NOW)
    #define FREE_LIBRARY(handle) dlclose(handle)
    #define GET_PROC_ADDRESS(handle, name) dlsym(handle, name)
#endif

namespace bootp::Plugins::DHCP::BootServices
{
	// Function pointer type for RegisterBootService
	typedef bool(*RegisterBootServiceFn)();

	std::vector<BootServiceLoader::LoadedDll>& BootServiceLoader::GetLoadedDlls()
	{
		static std::vector<LoadedDll> loadedDlls;
		return loadedDlls;
	}

	size_t BootServiceLoader::LoadFromDirectory(const std::string& path)
	{
		size_t loadedCount = 0;

#ifdef _WIN32
		WIN32_FIND_DATAA findData;
		HANDLE hFind = FindFirstFileA((path + "\\*.dll").c_str(), &findData);

		if (hFind == INVALID_HANDLE_VALUE)
		{
			return 0;
		}

		do
		{
			std::string dllPath = path + "\\" + std::string(findData.cFileName);
			if (LoadDll(dllPath))
			{
				loadedCount++;
			}
		} while (FindNextFileA(hFind, &findData));

		FindClose(hFind);
#else
		// Linux: use opendir/readdir
		DIR* dir = opendir(path.c_str());
		if (!dir)
		{
			return 0;
		}

		struct dirent* entry;
		while ((entry = readdir(dir)) != nullptr)
		{
			std::string name = entry->d_name;
			if (name.length() > 3 && name.substr(name.length() - 3) == ".so")
			{
				std::string dllPath = path + "/" + name;
				if (LoadDll(dllPath))
				{
					loadedCount++;
				}
			}
		}
		closedir(dir);
#endif

		return loadedCount;
	}

	bool BootServiceLoader::LoadDll(const std::string& dllPath)
	{
		// Load the DLL
		DLL_HANDLE handle = LOAD_LIBRARY(dllPath.c_str());
		if (!handle)
		{
#ifdef _WIN32
			std::cerr << "Failed to load DLL: " << dllPath 
					  << " (Error: " << GetLastError() << ")" << std::endl;
#else
			std::cerr << "Failed to load DLL: " << dllPath 
					  << " (Error: " << dlerror() << ")" << std::endl;
#endif
			return false;
		}

		// Get the RegisterBootService function
		RegisterBootServiceFn registerFn = 
			reinterpret_cast<RegisterBootServiceFn>(
				GET_PROC_ADDRESS(handle, "RegisterBootService"));

		if (!registerFn)
		{
			std::cerr << "Failed to find RegisterBootService in: " << dllPath << std::endl;
			FREE_LIBRARY(handle);
			return false;
		}

		// Call RegisterBootService
		if (!registerFn())
		{
			std::cerr << "RegisterBootService failed in: " << dllPath << std::endl;
			FREE_LIBRARY(handle);
			return false;
		}

		// Store the handle
		LoadedDll loadedDll;
		loadedDll.handle = handle;
		loadedDll.path = dllPath;
		GetLoadedDlls().push_back(loadedDll);

		return true;
	}

	void BootServiceLoader::UnloadAll()
	{
		for (auto& dll : GetLoadedDlls())
		{
			if (dll.handle)
			{
				FREE_LIBRARY(dll.handle);
				dll.handle = nullptr;
			}
		}
		GetLoadedDlls().clear();
	}

	size_t BootServiceLoader::GetLoadedCount()
	{
		return GetLoadedDlls().size();
	}
}
