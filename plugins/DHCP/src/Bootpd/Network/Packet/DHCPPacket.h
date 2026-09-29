#pragma once
#include "../../Bootpd.h"
#include "../../Defines/Definitions.h"

namespace bootp::Plugins::DHCP::Network::Packet
{
    class DHCPPacket : protected bootp::Network::Packet
    {
    public:
        DHCPPacket();
        explicit DHCPPacket(const char *data, const _SIZET &len);
        ~DHCPPacket() override;

        // DHCP Header fields (offset in packet)
        _UINT8 GetOperation() const;
        _UINT8 GetHardwareType() const;
        _UINT8 GetHardwareAddressLength() const;
        _UINT8 GetHops() const;
        _UINT32 GetTransactionID() const;
        _UINT16 GetSeconds() const;
        _UINT16 GetFlags() const;
        _UINT32 GetCIAddr() const;      // Client IP
        _UINT32 GetYIAddr() const;      // Your (assigned) IP
        _UINT32 GetSIAddr() const;      // Server IP
        _UINT32 GetGIAddr() const;      // Relay Agent IP (giaddr)
        // ... (chaddr, sname, file omitted for now)

        // Check if packet is relayed (giaddr != 0)
        bool IsRelayed() const { return GetGIAddr() != 0; }

        // For relayed packets: where to send response
        _UINT32 GetResponseAddr() const { return IsRelayed() ? GetGIAddr() : 0xFFFFFFFF; }
        _UINT16 GetResponsePort() const { return IsRelayed() ? 67 : 68; }  // 67=server, 68=client

        // Read DHCP options
        DHCPMessageType GetMessageType() const;

    private:
        static constexpr _UINT32 DHCP_MAGIC_COOKIE = 0x63825363;
    };
}
