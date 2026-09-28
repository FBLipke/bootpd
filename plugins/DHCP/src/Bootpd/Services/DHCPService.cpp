#include "DHCPService.h"

namespace bootp::Plugins
{
    bool DHCPService::Init()
    {
        printf("[D] DHCPService::Init()\n");

        return true;
    }

    bool DHCPService::Start()
    {
        printf("[D] DHCPService::Start()\n");
        return true;
    }

    void DHCPService::HeartBeat()
    {
    }

    void DHCPService::Close()
    {
    }

    void DHCPService::Handle_Service_Request(const _STRING &server, const _STRING &socket, const _STRING &client, const std::shared_ptr<bootp::Network::IPacket> &packet)
    {
    }
}
