#include "ServiceManager.h"

namespace bootp
{
    namespace Services
    {
        PluginLoader::PluginLoader() {}
        PluginLoader::~PluginLoader() { unload_all(); }

        bool PluginLoader::load(const _STRING &path)
        {
            printf("[D] Loading Libary: %s\n", path.c_str());

            PluginHandle handle = LoadLibraryA(path.c_str());

            if (!handle)
            {
                FreeLibrary(handle);
                return false;
            }

            auto create = (CreateFunc)GetProcAddress(handle, "create_plugin");
            if (!create)
            {
                FreeLibrary(handle);
                return false;
            }

            auto plugin = create();
            if (!plugin)
            {
                FreeLibrary(handle);
                return false;
            }

            if (!plugin->on_load())
            {
                FreeLibrary(handle);
                return false;
            }

            PluginEntry entry;
            entry.handle = handle;
            entry.instance.reset(plugin);
            m_plugins[plugin->name()] = std::move(entry);

            return true;
        }

        bool PluginLoader::load_from_dir(const _STRING &dir)
        {

#ifdef _WIN32
            // === WINDOWS: Alle .dll im Verzeichnis ===
            WIN32_FIND_DATAA find_data;

            auto hFind = FindFirstFileA((dir + "\\*" PLUGIN_EXT).c_str(), &find_data);
            if (hFind == INVALID_HANDLE_VALUE)
                return false;

            do
            {
                _STRING path = dir + "\\" + find_data.cFileName;
                load(path);
            } while (FindNextFileA(hFind, &find_data));
            FindClose(hFind);
#else
            // === LINUX: Alle .so im Verzeichnis ===
            DIR *d = opendir(dir.c_str());
            if (!d)
            {
                return false;
            }

            struct dirent *entry;
            while ((entry = readdir(d)) != nullptr)
            {
                _STRING name = entry->d_name;
                // Nur .so Dateien laden
                if (name.length() > 3 && name.substr(name.length() - 3) == PLUGIN_EXT)
                {
                    _STRING path = dir + "/" + name;
                    load(path);
                }
            }
            closedir(d);
#endif
            return true;
        }

        void PluginLoader::unload_all()
        {
            for (auto &pair : m_plugins)
            {
                pair.second.instance->on_unload();

                FreeLibrary(pair.second.handle);
            }
            m_plugins.clear();
        }

        IPlugin *PluginLoader::find(const _STRING &name)
        {
            auto it = m_plugins.find(name);
            if (it != m_plugins.end())
                return it->second.instance.get();

            return nullptr;
        }

        std::vector<_STRING> PluginLoader::list() const
        {
            std::vector<_STRING> names;
            for (const auto &pair : m_plugins)
                names.push_back(pair.first);

            return names;
        }

        bool ServiceManager::Init(const _INT32 &argc, const _BYTE *argv[])
        {
            printf("[D] Bootpd - ServiceMgr...\n");

            this->HandleManager_Request = [&](const _STRING &server_id, const _STRING &socket_id,
                                              const _BYTE *buffer, const _SIZET &length)
            {
                for (const auto &service : this->services)
                    service.second.get()->Handle_Service_Request();
            };

            this->LoadPlugins();

            for (const auto &service : this->services)
                service.second.get()->Init();

            return true;
        }

        bool ServiceManager::Start()
        {
            for (const auto &service : this->services)
                service.second.get()->Start();

            return true;
        }

        void ServiceManager::HeartBeat()
        {
            for (const auto &service : this->services)
                service.second.get()->HeartBeat();
        }

        void ServiceManager::LoadPlugins()
        {
            if (!this->pluginLoader.load_from_dir("plugins/"))
                return;

            for (const auto &name : this->pluginLoader.list())
            {
                auto plugin = this->pluginLoader.find(name);
                this->services.insert(std::make_pair(name, std::unique_ptr<IPlugin>(plugin)));
            }
        }

        void ServiceManager::Close()
        {
            for (const auto &service : this->services)
                service.second.get()->Close();
        }

        ServiceManager::ServiceManager()
        {
        }

        ServiceManager::~ServiceManager()
        {
        }
    }
}
