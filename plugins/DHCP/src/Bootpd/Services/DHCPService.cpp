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
}
