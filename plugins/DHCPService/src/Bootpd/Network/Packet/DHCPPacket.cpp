#include "DHCPPacket.h"

namespace bootp::Plugins::DHCP::Network::Packet
{
    bootp::Plugins::DHCP::BootpOPCode DHCPPacket::Get_OPCode()
    {
        return static_cast<BootpOPCode>(Read<_BYTE>(0));
    }

    bootp::Plugins::DHCP::HardwareType DHCPPacket::Get_HWType()
    {
        return static_cast<HardwareType>(Read<_BYTE>(1));
    }

}