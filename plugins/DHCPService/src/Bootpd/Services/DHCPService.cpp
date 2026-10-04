#include "DHCPService.h"

extern "C"
{
	__LIBEXPORT bootp::IPlugin *create_plugin()
	{
		return new bootp::Plugins::DHCP::DHCPService();
	}
}

namespace bootp::Plugins::DHCP
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

	void DHCPService::configure(const tinyxml2::XMLDocument &doc, IBootpd *parent)
	{
		auto service = doc.RootElement()->FirstChildElement("Configuration")->FirstChildElement("Services")->FirstChildElement("Service");

		while (service)
		{
			auto type = std::string(service->Attribute("type"));

			if (type.compare(this->name()) == 0)
			{
				std::vector<_USHORT> ports;
				std::string port_str = service->Attribute("port");
				std::string token;
				std::istringstream iss(port_str);
				while (std::getline(iss, token, SPLITTOKEN))
					ports.push_back(static_cast<_USHORT>(std::stoi(token)));

				auto mcaddr = service->Attribute("mcaddr");
				auto mccport = service->Attribute("mccport");
				auto mcsport = service->Attribute("mcsport");
				auto mcstartdelay = service->Attribute("mcstartdelay");
				auto mctimeout = service->Attribute("mctimeout");
				auto discovery = service->Attribute("discovery");
				auto menuetimeout = service->Attribute("menuetimeout");
				auto menueprompt = service->Attribute("menueprompt");

				if (ports.empty() == false)
				{
					parent->Get_SubSystem("ServerManager")->Add_Server(ports);
					break;
				}
			}

			service = service->NextSiblingElement("Service");
		}
	}

	void DHCPService::Handle_Service_Request(const _STRING &server, const _STRING &socket,
											 const _STRING &client, const std::shared_ptr<bootp::Network::IPacket> &packet)
	{
		auto dhcp_packet = std::dynamic_pointer_cast<bootp::Plugins::DHCP::Network::Packet::DHCPPacket>(packet);
		if (!dhcp_packet)
		{
			printf("[E] DHCPService: Received packet is not a DHCP packet.\n");
			return;
		}

		auto opcode = dhcp_packet->Get_OPCode();

		switch (opcode)
		{
		case BootpOPCode::BootRequest:
			printf("[D] DHCPService: Received BootRequest packet.\n");
			break;
		case BootpOPCode::BootReply:
			printf("[D] DHCPService: Received BootReply packet.\n");
			break;
		default:
			printf("[D] DHCPService: Received packet with unknown opcode.\n");
			break;
		}

		auto hwtype = dhcp_packet->Get_HWType();
	}
}
