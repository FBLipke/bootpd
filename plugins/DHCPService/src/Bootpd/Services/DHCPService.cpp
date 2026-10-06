#include "DHCPService.h"
#include "../Network/Packet/DHCPPacket.h"
#include "../BootServices/IBootService.h"
#include "../BootServices/BootServiceLoader.h"

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

		BootServiceLoader::LoadFromDirectory("plugins/DHCPServices/");
	}

	void DHCPService::Handle_Service_Request(const _STRING &server, const _STRING &socket,
											 const _STRING &client, const std::shared_ptr<bootp::Network::IPacket> &packet)
	{
		auto request = std::reinterpret_pointer_cast<bootp::Plugins::DHCP::Network::Packet::DHCPPacket>(packet);
		if (!request)
		{
			printf("[E] DHCPService: Received packet is not a DHCP packet.\n");
			return;
		}

		if (request->IsRelayedPacket())
		{
			printf("[I] Got Relayed Request Packet!\n");
		}

		auto opcode = request->Get_OPCode();

		switch (opcode)
		{
		case BootpOPCode::BootRequest:
			printf("[D] DHCPService: Received BootRequest packet.\n");
			switch (request->Get_MessageType())
			{
			case DHCPMessageType::Discover:
				Handle_DHCP_Discover(server, socket, client, request);
				break;
			case DHCPMessageType::Request:
				Handle_DHCP_Request(server, socket, client, request);
				break;
			case DHCPMessageType::Release:
				Handle_DHCP_Release(server, socket, client, request);
				break;
			case DHCPMessageType::Inform:
				Handle_DHCP_Inform(server, socket, client, request);
				break;
			default:
				break;
			}

			break;
		case BootpOPCode::BootReply:
			printf("[D] DHCPService: Received BootReply packet.\n");
			switch (request->Get_MessageType())
			{
			case DHCPMessageType::Offer:
				Handle_DHCP_Offer(server, socket, client, request);
				break;
			case DHCPMessageType::Ack:
				Handle_DHCP_Ack(server, socket, client, request);
				break;
			case DHCPMessageType::Nak:
				Handle_DHCP_Nak(server, socket, client, request);
				break;
			default:
				break;
			}
			break;
		default:
			printf("[D] DHCPService: Received packet with unknown opcode.\n");
			break;
		}

		auto hwtype = request->Get_HWType();
	}

	void DHCPService::Handle_DHCP_Discover(const _STRING &server, const _STRING &socket, const _STRING &client,
										   const std::shared_ptr<bootp::Plugins::DHCP::Network::Packet::DHCPPacket> &request)
	{
		// Find passenden BootService
		auto service = BootServiceRegistry::Instance().FindService(request);
		if (service)
		{
			service->OnDiscover(server, socket, client, request);
		}
	}

	void DHCPService::Handle_DHCP_Request(const _STRING &server, const _STRING &socket, const _STRING &client,
										  const std::shared_ptr<bootp::Plugins::DHCP::Network::Packet::DHCPPacket> &request)
	{
	}

	void DHCPService::Handle_DHCP_Release(const _STRING &server, const _STRING &socket, const _STRING &client,
										  const std::shared_ptr<bootp::Plugins::DHCP::Network::Packet::DHCPPacket> &request)
	{
	}

	void DHCPService::Handle_DHCP_Inform(const _STRING &server, const _STRING &socket, const _STRING &client,
										 const std::shared_ptr<bootp::Plugins::DHCP::Network::Packet::DHCPPacket> &request)
	{
	}

	void DHCPService::Handle_DHCP_Offer(const _STRING &server, const _STRING &socket, const _STRING &client,
										const std::shared_ptr<bootp::Plugins::DHCP::Network::Packet::DHCPPacket> &request)
	{
	}

	void DHCPService::Handle_DHCP_Ack(const _STRING &server, const _STRING &socket, const _STRING &client,
									  const std::shared_ptr<bootp::Plugins::DHCP::Network::Packet::DHCPPacket> &request)
	{
	}

	void DHCPService::Handle_DHCP_Nak(const _STRING &server, const _STRING &socket, const _STRING &client,
									  const std::shared_ptr<bootp::Plugins::DHCP::Network::Packet::DHCPPacket> &request)
	{
	}
}
