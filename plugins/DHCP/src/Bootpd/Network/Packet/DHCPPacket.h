#pragma once
#include "../../Bootpd.h"

namespace bootp::Plugins::DHCP::Network::Packet
{
    class DHCPPacket : protected bootp::Network::Packet
    {
    private:
    public:
        DHCPPacket(/* args */);
        ~DHCPPacket() override;
    };
}
