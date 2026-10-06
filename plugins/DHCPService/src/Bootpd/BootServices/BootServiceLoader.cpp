#include "BootServiceLoader.h"
#include <iostream>
#include <cstdlib>

#ifndef _WIN32
#include <dirent.h>
#endif

#ifdef _WIN32
#include <windows.h>
#define DLOPEN(path) LoadLibraryA(path.c_str())
#define DLSYM(handle, sym) GetProcAddress(handle, sym)
#define DLCLOSE(handle) FreeLibrary(handle)
#define DLERROR() std::to_string(GetLastError())
#else
#include <dlfcn.h>
#define DLOPEN(path) dlopen(path.c_str(), RTLD_NOW)
#define DLSYM(handle, sym) dlsym(handle, sym)
#define DLCLOSE(handle) dlclose(handle)
#define DLERROR() dlerror()
#endif

namespace bootp::Plugins::DHCP::BootServices
{
	typedef bool (*RegisterBootServiceFn)();

	static std::vector<void *> &GetLoadedHandles()
	{
		static std::vector<void *> handles;
		return handles;
	}

	size_t BootServiceLoader::LoadFromDirectory(const std::string &path)
	{
		size_t loadedCount = 0;

		// Find files matching *.so (Linux) or *.dll (Windows)
		std::string pattern;
#ifdef _WIN32
		pattern = path + "\\*.dll";
#else
		pattern = path + "/*.so";
#endif

		// Use glob or FindFirstFile equivalent
#ifdef _WIN32
		WIN32_FIND_DATAA findData;
		HANDLE hFind = FindFirstFileA(pattern.c_str(), &findData);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			return 0;
		}
		do
		{
			std::string dllPath = path + "\\" + std::string(findData.cFileName);
#else
		DIR *dir = opendir(path.c_str());
		if (!dir)
		{
			return 0;
		}
		struct dirent *entry;
		while ((entry = readdir(dir)) != nullptr)
		{
			std::string filename(entry->d_name);
			// Skip if doesn't end in .so
			if (filename.size() < 3 || filename.substr(filename.size() - 3) != ".so")
			{
				continue;
			}
			std::string dllPath = path + "/" + filename;
#endif

			void *handle = DLOPEN(dllPath);
			if (!handle)
			{
				std::cerr << "Failed to load DLL: " << dllPath
						  << " (Error: " << DLERROR() << ")" << std::endl;
				continue;
			}
			/*
						auto registerFn = (RegisterBootServiceFn)DLSYM(handle, "RegisterBootService");
						if (!registerFn)
						{
							std::cerr << "Failed to find RegisterBootService in: " << dllPath << std::endl;
							DLCLOSE(handle);
							continue;
						}

						if (!registerFn())
						{
							std::cerr << "RegisterBootService failed in: " << dllPath << std::endl;
							DLCLOSE(handle);
							continue;
						}
						*/
			GetLoadedHandles().push_back(handle);
			loadedCount++;

#ifdef _WIN32
		} while (FindNextFileA(hFind, &findData));
		FindClose(hFind);
#else
		}
		closedir(dir);
#endif

		return loadedCount;
	}

	void BootServiceLoader::UnloadAll()
	{
		/*
		for (void *handle : GetLoadedHandles())
		{
			if (handle)
			{
				DLCLOSE(handle);
			}
		}
		GetLoadedHandles().clear();
		*/
	}

	size_t BootServiceLoader::GetLoadedCount()
	{
		return GetLoadedHandles().size();
	}
}
