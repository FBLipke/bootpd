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

	void DHCPPacket::Set_Hops(const _BYTE &hops)
	{
		Write(3, static_cast<_BYTE>(hops));
	}

	_BYTE DHCPPacket::Get_Hops()
	{
		return Read<_BYTE>(3);
	}

	void DHCPPacket::Set_Xid(const _ULONG &xid)
	{
		Write<_ULONG>(4, static_cast<_ULONG>(xid));
	}

	_ULONG DHCPPacket::Get_Xid()
	{
		return Read<_ULONG>(4);
	}

	void DHCPPacket::Set_Secs(const _USHORT &secs)
	{
		Write<_USHORT>(8, static_cast<_USHORT>(secs));
	}

	_USHORT DHCPPacket::Get_Secs()
	{
		return Read<_USHORT>(8);
	}

	void DHCPPacket::Set_Flags(const DHCPFlags &flags)
	{
		Write(htons(static_cast<_USHORT>(flags)), 10);
	}

	DHCPFlags DHCPPacket::Get_Flags()
	{
		return static_cast<DHCPFlags>(Read<_USHORT>(10));
	}

	void DHCPPacket::Set_ClientIP(const _IPADDR &ip)
	{
		Write<_IPADDR>(12, ip);
	}

	_IPADDR DHCPPacket::Get_ClientIP()
	{
		return Read<_IPADDR>(12);
	}

	void DHCPPacket::Set_YourIP(const _IPADDR &ip)
	{
		Write<_IPADDR>(16, ip);
	}

	_IPADDR DHCPPacket::Get_YourIP()
	{
		return Read<_IPADDR>(16);
	}

	void DHCPPacket::Set_NextIP(const _IPADDR &ip)
	{
		Write<_IPADDR>(20, ip);
	}

	_IPADDR DHCPPacket::Get_NextIP()
	{
		return Read<_IPADDR>(20);
	}

	void DHCPPacket::Set_RelayIP(const _IPADDR &ip)
	{
		Write<_IPADDR>(24, ip);
	}

	_IPADDR DHCPPacket::Get_RelayIP()
	{
		return Read<_IPADDR>(24);
	}

	_BOOL DHCPPacket::IsRelayedPacket()
	{
		return this->Get_RelayIP() != 0;
	}

	void DHCPPacket::Set_HWAddress(const char *buffer, const _SIZET &length)
	{
		auto hwtype = this->Get_HWType();

		switch (hwtype)
		{
		case HardwareType::Infiniband:
			char hwaddr[16];
			_ClearBuffer(hwaddr, sizeof(hwaddr));
			Write(hwaddr, 28, 16);

			this->AddOption(DHCPOption(61, buffer, length));

			break;
		default:
			Write(buffer, 28, length);
			break;
		}
	}

	void DHCPPacket::Get_HWAddress(char *out, const _SIZET &length)
	{
		_ClearBuffer(out, length);

		Read(28, out, length);
	}

	DHCPMessageType DHCPPacket::Get_MessageType()
	{
		return static_cast<DHCPMessageType>(this->_optionbuffer.at(53).GetData()[0]);
	}
}
