#pragma once
#include "../../Bootpd.h"
#include "../../Defines/Definitions.h"
#include "../../Defines/DHCPOption.h"

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

		_BOOL IsRelayedPacket();

		void Set_Hops(const _BYTE &hops);
		_BYTE Get_Hops();

		void Set_Xid(const _ULONG &xid);
		_ULONG Get_Xid();

		void Set_Secs(const _USHORT &secs);
		_USHORT Get_Secs();

		void Set_Flags(const DHCPFlags &flags);
		DHCPFlags Get_Flags();

		void Set_ClientIP(const _IPADDR &ip);
		_IPADDR Get_ClientIP();

		void Set_YourIP(const _IPADDR &ip);
		_IPADDR Get_YourIP();

		void Set_NextIP(const _IPADDR &ip);
		_IPADDR Get_NextIP();

		void Set_RelayIP(const _IPADDR &ip);
		_IPADDR Get_RelayIP();

		void Set_HWAddress(const char *buffer, const _SIZET &length);
		void Get_HWAddress(char *out, const _SIZET &length);

		DHCPMessageType Get_MessageType();

		template <typename T>
		void AddOption(const _BYTE &code, T data, const _BOOL &append = false)
		{
			if (this->_optionbuffer.count(code) == 1)
			{
				if (append)
				{
					this->_optionbuffer.at(code).AddData(data);
					return;
				}
				else
					this->_optionbuffer.erase(code);
			}

			this->_optionbuffer.insert_or_assign(code, data);
		}

		void AddOption(const DHCPOption &opt, const _BOOL &append = false)
		{
			if (this->_optionbuffer.count(opt.GetCode()) == 1)
			{
				if (append)
				{
					this->_optionbuffer.at(opt.GetCode()).AddData(opt.GetData());
					return;
				}
				else
					this->_optionbuffer.erase(opt.GetCode());
			}

			this->_optionbuffer.insert_or_assign(opt.GetCode(), opt);
		}

	private:
		/*
		 *  Option 43, Len = 255, Data[0 - 254]
		 *  Option 43, Len = 45, Data[255 - 8192]
		 */
		std::map<_BYTE, DHCPOption> _optionbuffer;
	};
}
