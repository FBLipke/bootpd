/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once
#include "../Bootpd.h"

namespace bootp
{
    enum class PluginType
    {
        CORE,    // DHCP, TFTP
        SERVICE, // WDS, RIS, BSDP
        NETWORK,
        OTHER
    };

    /*
     * IPlugin - Base Interface für alle Plugins
     */
    class IPlugin
    {
    public:
        virtual ~IPlugin() = default;

        // Plugin Info
        virtual _STRING name() const = 0;
        virtual PluginType type() const = 0;
        virtual _INT32 priority() const = 0; // Load order

        // Plugin Instance
        virtual _BOOL Init() = 0;
        virtual _BOOL Start() = 0;
        virtual void HeartBeat() = 0;
        virtual void Close() = 0;

        virtual void Handle_Service_Request() = 0;

        // Lifecycle
        virtual _BOOL on_load() = 0;
        virtual void on_unload() = 0;

        // Configuration
        virtual void configure(const _STRING &key, const _STRING &value) = 0;
    };

    typedef IPlugin *(*CreateFunc)();

    extern "C"
    {
        // Diese Funktion MUSS in jeder Plugin-DLL exportiert werden
        bootp::IPlugin *create_plugin();
    }
    namespace Services
    {

        class PluginLoader
        {
        public:
            PluginLoader();
            ~PluginLoader();

            // Einzelne Plugin-Datei laden
            _BOOL load(const _STRING &path);

            // Alle Plugins aus Verzeichnis laden
            _BOOL load_from_dir(const _STRING &dir);

            // Alle Plugins entladen
            void unload_all();

            // Plugin nach Namen finden
            bootp::IPlugin *find(const _STRING &name);

            // Anzahl geladener Plugins
            _SIZET count() const { return m_plugins.size(); }

            // Plugin-Liste
            std::vector<_STRING> list() const;

        private:
            struct PluginEntry
            {
                PluginHandle handle;
                std::unique_ptr<bootp::IPlugin> instance;
            };

            std::map<_STRING, PluginEntry> m_plugins;
        };

        class ServiceManager : public IBootpd
        {
        public:
            ServiceManager();
            ~ServiceManager();

            _BOOL Init(const _INT32 &argc, const _BYTE *argv[]);
            _BOOL Start();
            void HeartBeat();
            void LoadPlugins();
            void Close();

        private:
            PluginLoader pluginLoader;
            std::map<const _STRING, std::unique_ptr<bootp::IPlugin>> services;
        };
    }
}