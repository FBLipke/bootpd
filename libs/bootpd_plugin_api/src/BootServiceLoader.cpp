#include "bootp/plugins/api/BootServiceLoader.h"
#include <iostream>

#ifdef _WIN32
	#include <windows.h>
	#define DLOPEN(path) LoadLibraryA(path)
	#define DLSYM(handle, sym) GetProcAddress(handle, sym)
	#define DLCLOSE(handle) FreeLibrary(handle)
	#define DLERROR() std::to_string(GetLastError())
#else
	#include <dlfcn.h>
	#include <dirent.h>
	#define DLOPEN(path) dlopen(path, RTLD_NOW)
	#define DLSYM(handle, sym) dlsym(handle, sym)
	#define DLCLOSE(handle) dlclose(handle)
	#define DLERROR() dlerror()
#endif

namespace bootp::plugins::api
{
	static std::vector<void*>& GetLoadedHandles()
	{
		static std::vector<void*> handles;
		return handles;
	}

	size BootServiceLoader::LoadFromDirectory(const std::string& path)
	{
		size loadedCount = 0;

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
			if (!LoadPlugin(dllPath))
			{
				continue;
			}
			loadedCount++;
		} while (FindNextFileA(hFind, &findData));
		FindClose(hFind);
	#else
		DIR* dir = opendir(path.c_str());
		if (!dir)
		{
			return 0;
		}
		struct dirent* entry;
		while ((entry = readdir(dir)) != nullptr)
		{
			std::string filename(entry->d_name);
			if (filename.size() < 3 || filename.substr(filename.size() - 3) != ".so")
			{
				continue;
			}
			std::string dllPath = path + "/" + filename;
			if (!LoadPlugin(dllPath))
			{
				continue;
			}
			loadedCount++;
		}
		closedir(dir);
	#endif

		return loadedCount;
	}

	bool BootServiceLoader::LoadPlugin(const std::string& path)
	{
		void* handle = DLOPEN(path.c_str());
		if (!handle)
		{
			std::cerr << "Failed to load plugin: " << path << " (Error: " << DLERROR() << ")" << std::endl;
			return false;
		}

		auto registerFn = (RegisterBootServiceFn)DLSYM((HMODULE)handle, "RegisterBootService");
		if (!registerFn)
		{
			std::cerr << "Failed to find RegisterBootService in: " << path << std::endl;
			DLCLOSE((HMODULE)handle);
			return false;
		}

		if (!registerFn())
		{
			std::cerr << "RegisterBootService failed in: " << path << std::endl;
			DLCLOSE((HMODULE)handle);
			return false;
		}

		GetLoadedHandles().push_back(handle);
		return true;
	}

	void BootServiceLoader::UnloadAll()
	{
		for (void* handle : GetLoadedHandles())
		{
			if (handle)
			{
				DLCLOSE((HMODULE)handle);
			}
		}
		GetLoadedHandles().clear();
	}

	size BootServiceLoader::GetLoadedCount()
	{
		return GetLoadedHandles().size();
	}
}
