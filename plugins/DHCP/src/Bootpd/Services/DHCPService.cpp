#include "DHCPService.h"

extern "C"
{
    __LIBEXPORT bootp::IPlugin *create_plugin()
    {
        return new bootp::Plugins::DHCPService();
    }
}

namespace bootp::Plugins
{
    bool DHCPService::Init()
    {
        return true;
    }

    bool DHCPService::Start()
    {
        return true;
    }

    void DHCPService::HeartBeat()
    {
    }

    void DHCPService::Close()
    {
    }

    bool DHCPService::on_load()
    {

        return this->Init();
    }

    void DHCPService::on_unload()
    {
        this->Close();
    }

    void DHCPService::on_install()
    {
    }

    void DHCPService::configure(const std::string &key, const std::string &value)
    {
    }

    void DHCPService::Handle_Service_Request(const _STRING &server, const _STRING &socket,
                                             const _STRING &client, const std::shared_ptr<bootp::Network::IPacket> &packet)
    {
    }
}
