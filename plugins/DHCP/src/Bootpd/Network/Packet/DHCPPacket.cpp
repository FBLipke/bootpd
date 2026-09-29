#include "DHCPPacket.h"

namespace bootp::Plugins::DHCP::Network::Packet
{
    DHCPPacket::DHCPPacket() : Packet()
    {
    }

    DHCPPacket::DHCPPacket(const char *data, const _SIZET &len) : Packet(data, len)
    {
    }

    DHCPPacket::~DHCPPacket()
    {
    }
}