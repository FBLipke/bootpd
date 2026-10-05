#pragma once
#include "../Bootpd.h"
#include <Bootpd/Services/ServiceManager.h>
#include "../Defines/DHCPOption.h"
#include "../Defines/Definitions.h"

namespace bootp::Plugins::DHCP
{
	class DHCPPacket;  // Forward declaration to break circular dependency

	class DHCPService : public IPlugin
	{
	public:
		static IPlugin *create() { return new DHCPService(); }
		// IPlugin interface
		std::string name() const override { return "DHCPService"; }
		PluginType type() const override { return PluginType::NETWORK; }
		int priority() const override { return 100; }

		bool Init() override;
		bool Start() override;
		void HeartBeat() override;
		void Close() override;

		__LIBEXPORT bool on_load() override;
		__LIBEXPORT void on_unload() override;
		__LIBEXPORT void on_install() override;

		__LIBEXPORT void configure(const tinyxml2::XMLDocument &doc, IBootpd *parent) override;

		void Handle_Service_Request(const _STRING &server, const _STRING &socket,
									const _STRING &client, const std::shared_ptr<bootp::Network::IPacket> &request) override;

		void Handle_DHCP_Discover(const _STRING server, const _STRING socket, const _STRING client, const DHCPPacket &request);

	private:
	};
}
