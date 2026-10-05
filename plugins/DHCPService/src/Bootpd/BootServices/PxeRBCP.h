#pragma once
#include "IBootService.h"
#include "PXEOptions.h"

namespace bootp::Plugins::DHCP::BootServices
{
	/**
	 * @brief PxeRBCP - PXE Remote Boot Control Protocol Boot Service
	 * 
	 * This is the DEFAULT BootService that intercepts PXEClient Discover requests.
	 * It generates a boot menu from all registered BootServers and sends it
	 * to the client. When the user selects an option, the request is forwarded
	 * to the appropriate BootService.
	 * 
	 * Flow:
	 * 1. Client sends DHCP Discover with Option 60 = "PXEClient"
	 * 2. RBCP catches it, builds menu from all registered BootServers
	 * 3. Client shows menu, user selects option
	 * 4. Client sends DHCP Request with selected BootServerType
	 * 5. Appropriate BootService handles the request
	 */
	class PxeRBCP : public IBootService
	{
	public:
		PxeRBCP();
		~PxeRBCP() override = default;

		// IBootService interface
		BootServerType GetServerType() const override;
		_STRING GetName() const override;
		bool CanHandle(const DHCPPacket& request) const override;
		bool OnDiscover(const _STRING& server, const _STRING& socket,
					   const _STRING& client, const DHCPPacket& request) override;
		bool OnRequest(const _STRING& server, const _STRING& socket,
					 const _STRING& client, const DHCPPacket& request) override;
		void OnRelease(const _STRING& server, const _STRING& socket,
					 const _STRING& client, const DHCPPacket& request) override;
		void OnInform(const _STRING& server, const _STRING& socket,
					 const _STRING& client, const DHCPPacket& request) override;

		// RBCP specific methods
		void SetDiscoveryControl(_BYTE value);
		void SetMenueTimeout(_BYTE seconds);
		void SetMenuePrompt(const std::vector<_STRING>& prompts);
		void SetMulticastDelay(_BYTE seconds);
		void SetMulticastTimeout(_BYTE seconds);
		void SetMulticastAddress(const _UBYTE* ip);
		void SetMulticastPorts(_WORD serverPort, _WORD clientPort);

		// Register a boot server to show in menu
		void RegisterBootServer(BootServerType type, const _STRING& hostname, const _UBYTE* ip);

	private:
		// Generate PXE Option 43 data
		std::vector<char> GeneratePXEOptions() const;
		
		// Generate BootServer list (Option 43, sub-option 8)
		DHCPOption GenerateBootServersList() const;
		
		// Generate Boot Menu (Option 43, sub-option 77)
		DHCPOption GenerateBootMenue() const;
		
		// Generate Menu Prompt (Option 43, sub-option 79)
		DHCPOption GenerateBootMenuePrompt() const;

		// RBCP Settings
		_BYTE discoveryControl = 3;
		_BYTE menueTimeout = 10;
		std::vector<_STRING> menuePrompt = { "Select Server...", "Press [F8] to boot from Network or [esc] to cancel..." };
		_BYTE multicastDelay = 4;
		_BYTE multicastTimeout = 10;
		_UBYTE multicastAddress[4] = { 224, 0, 1, 2 };  // Default: 224.0.1.2
		_WORD multicastServerPort = 69;
		_WORD multicastClientPort = 4001;

		// Registered boot servers for the menu
		std::map<BootServerType, std::pair<_STRING, _UBYTE[4]>> bootServers;
	};
}
