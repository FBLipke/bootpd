#pragma once
#include "../Bootpd.h"
// IPlugin basis class from CMake include dirs: ${CMAKE_SOURCE_DIR}/bootpd/src
#include <Bootpd/Services/ServiceManager.h>

namespace bootp::Plugins
{

    class DHCPService : public IPlugin
    {
    public:
        // IPlugin interface
        std::string name() const override { return "DHCP"; }
        PluginType type() const override { return PluginType::NETWORK; }
        int priority() const override { return 100; }

        bool Init() override;
        bool Start() override;
        void HeartBeat() override;
        void Close() override;

        bool on_load() override;
        void on_unload() override;

        void configure(const std::string &key, const std::string &value) override;

        void Handle_Service_Request(const _STRING &server, const _STRING &socket, const _STRING &client, const std::shared_ptr<bootp::Network::IPacket> &packet);

    private:
    };
}
