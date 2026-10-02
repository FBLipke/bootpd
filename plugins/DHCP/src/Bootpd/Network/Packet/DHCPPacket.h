#pragma once
#include "../../Bootpd.h"
#include "../../Defines/Definitions.h"

namespace bootp::Plugins::DHCP::Network::Packet
{
    class DHCPPacket : protected bootp::Network::Packet
    {
    public:
        DHCPPacket() : Packet() {}
        explicit DHCPPacket(const char *data, const _SIZET &len) : Packet(data, len) {}
        ~DHCPPacket() {}

        bootp::Plugins::DHCP::BootpOPCode Get_OPCode();
        bootp::Plugins::DHCP::HardwareType Get_HWType();
    };
}
