#include "PxeRBCP.h"

namespace bootp::Plugins::DHCP::BootServices
{
	PxeRBCP::PxeRBCP()
	{
		// Register this service as the default PXE Bootstrap Server
		BootServiceRegistry::Instance().Register(std::make_unique<PxeRBCP>(*this));
	}

	BootServerType PxeRBCP::GetServerType() const
	{
		return BootServerType::PXEBootstrapServer;  // Will be overridden in configure
	}

	_STRING PxeRBCP::GetName() const
	{
		return "PxeRBCP";
	}

	bool PxeRBCP::CanHandle(const DHCPPacket& request) const
	{
		// RBCP handles all PXEClient discovers as the default bootstrap
		// But only if no specific BootItem is selected yet
		return true;  // Will be refined based on DHCP message type
	}

	bool PxeRBCP::OnDiscover(const _STRING& server, const _STRING& socket,
							 const _STRING& client, const DHCPPacket& request)
	{
		// Generate the full PXE menu response
		auto pxedata = GeneratePXEOptions();
		
		// Add to response packet (this would be handled by DHCPService)
		// For now, just log
		return true;
	}

	bool PxeRBCP::OnRequest(const _STRING& server, const _STRING& socket,
						   const _STRING& client, const DHCPPacket& request)
	{
		// If we get here, the client has selected a BootItem
		// Find the appropriate BootService and forward
		auto service = BootServiceRegistry::Instance().FindService(request);
		if (service && service != this)
		{
			return service->OnRequest(server, socket, client, request);
		}
		return false;
	}

	void PxeRBCP::OnRelease(const _STRING& server, const _STRING& socket,
						   const _STRING& client, const DHCPPacket& request)
	{
		// Not typically handled by RBCP
	}

	void PxeRBCP::OnInform(const _STRING& server, const _STRING& socket,
						  const _STRING& client, const DHCPPacket& request)
	{
		// Not typically handled by RBCP
	}

	void PxeRBCP::SetDiscoveryControl(_BYTE value)
	{
		discoveryControl = value;
	}

	void PxeRBCP::SetMenueTimeout(_BYTE seconds)
	{
		menueTimeout = seconds;
	}

	void PxeRBCP::SetMenuePrompt(const std::vector<_STRING>& prompts)
	{
		menuePrompt = prompts;
	}

	void PxeRBCP::SetMulticastDelay(_BYTE seconds)
	{
		multicastDelay = seconds;
	}

	void PxeRBCP::SetMulticastTimeout(_BYTE seconds)
	{
		multicastTimeout = seconds;
	}

	void PxeRBCP::SetMulticastAddress(const _UBYTE* ip)
	{
		if (ip) {
			multicastAddress[0] = ip[0];
			multicastAddress[1] = ip[1];
			multicastAddress[2] = ip[2];
			multicastAddress[3] = ip[3];
		}
	}

	void PxeRBCP::SetMulticastPorts(_USHORT serverPort, _USHORT clientPort)
	{
		multicastServerPort = serverPort;
		multicastClientPort = clientPort;
	}

	void PxeRBCP::RegisterBootServer(BootServerType type, const _STRING& hostname, const _UBYTE* ip)
	{
		_UBYTE addr[4] = { 0, 0, 0, 0 };
		if (ip) {
			addr[0] = ip[0];
			addr[1] = ip[1];
			addr[2] = ip[2];
			addr[3] = ip[3];
		}
		bootServers[type] = std::make_pair(hostname, addr);
	}

	std::vector<char> PxeRBCP::GeneratePXEOptions() const
	{
		std::vector<char> options;

		// Discovery Control
		options.push_back(PXEOptions::DiscoveryControl);
		options.push_back(1);  // Length
		options.push_back(discoveryControl);

		// Multicast TFTPDelay
		options.push_back(PXEOptions::MulticastTFTPDelay);
		options.push_back(1);
		options.push_back(multicastDelay);

		// Multicast TFTPDTimeout
		options.push_back(PXEOptions::MulticastTFTPTimeout);
		options.push_back(1);
		options.push_back(multicastTimeout);

		// Multicast Address
		options.push_back(PXEOptions::DiscoveryMulticastAddress);
		options.push_back(4);
		for (int i = 0; i < 4; ++i)
			options.push_back(static_cast<char>(multicastAddress[i]));

		// Multicast Server Port
		options.push_back(PXEOptions::MulticastServerPort);
		options.push_back(2);
		options.push_back(static_cast<char>(multicastServerPort & 0xFF));
		options.push_back(static_cast<char>((multicastServerPort >> 8) & 0xFF));

		// Multicast Client Port
		options.push_back(PXEOptions::MulticastClientPort);
		options.push_back(2);
		options.push_back(static_cast<char>(multicastClientPort & 0xFF));
		options.push_back(static_cast<char>((multicastClientPort >> 8) & 0xFF));

		// Boot Servers List
		auto bootServersData = GenerateBootServersList();
		options.insert(options.end(), bootServersData.GetData().begin(), bootServersData.GetData().end());

		// Boot Menu
		auto bootMenueData = GenerateBootMenue();
		options.insert(options.end(), bootMenueData.GetData().begin(), bootMenueData.GetData().end());

		// Menu Prompt
		auto menuPromptData = GenerateBootMenuePrompt();
		options.insert(options.end(), menuPromptData.GetData().begin(), menuPromptData.GetData().end());

		// End
		options.push_back(PXEOptions::End);
		options.push_back(0);

		return options;
	}

	DHCPOption PxeRBCP::GenerateBootServersList() const
	{
		std::vector<char> serverList;

		// Sub-option header for BootServer (8)
		serverList.push_back(PXEOptions::BootServer);

		// Placeholder for length (will be calculated)
		size_t lengthPos = serverList.size();
		serverList.push_back(0);  // Placeholder

		for (const auto& [type, info] : bootServers)
		{
			const auto& [hostname, ip] = info;
			
			// Type (2 bytes, little-endian)
			serverList.push_back(static_cast<char>(static_cast<_USHORT>(type) & 0xFF));
			serverList.push_back(static_cast<char>((static_cast<_USHORT>(type) >> 8) & 0xFF));

			// Length of this entry
			_BYTE entryLen = 1 + 4 + static_cast<_BYTE>(hostname.length()) + 1;
			serverList.push_back(entryLen);

			// IP count
			serverList.push_back(1);

			// IP address
			for (int i = 0; i < 4; ++i)
				serverList.push_back(static_cast<char>(ip[i]));

			// Hostname (null-terminated)
			serverList.insert(serverList.end(), hostname.begin(), hostname.end());
			serverList.push_back(0);
		}

		// Update length at lengthPos
		serverList[lengthPos] = static_cast<char>(serverList.size() - lengthPos - 1);

		return DHCPOption(static_cast<_BYTE>(PXEOptions::BootServer), serverList);
	}

	DHCPOption PxeRBCP::GenerateBootMenue() const
	{
		std::vector<char> menu;

		// Sub-option header for BootMenue (77)
		menu.push_back(PXEOptions::BootMenue);

		// Placeholder for length
		size_t lengthPos = menu.size();
		menu.push_back(0);

		// Add "Local Boot" as first entry (type 0)
		menu.push_back(0);  // Type low byte
		menu.push_back(0);  // Type high byte
		menu.push_back(11); // Length: "Local Boot" + null = 10 + 1
		const char* localBoot = "Local Boot";
		menu.insert(menu.end(), localBoot, localBoot + 10);
		menu.push_back(0);  // Null terminator

		// Add registered boot servers
		for (const auto& [type, info] : bootServers)
		{
			const auto& [hostname, ip] = info;
			
			// Skip PXE Bootstrap Server type in menu (it's us!)
			if (type == BootServerType::PXEBootstrapServer)
				continue;

			// Type (2 bytes, little-endian)
			menu.push_back(static_cast<char>(static_cast<_USHORT>(type) & 0xFF));
			menu.push_back(static_cast<char>((static_cast<_USHORT>(type) >> 8) & 0xFF));

			// Build display string: "[hostname] TypeName"
			std::string displayText = "[" + hostname + "] " + GetServerTypeName(type);
			
			// Length
			menu.push_back(static_cast<char>(displayText.length() + 1));

			// Display text
			menu.insert(menu.end(), displayText.begin(), displayText.end());
			menu.push_back(0);  // Null terminator
		}

		// Update length
		menu[lengthPos] = static_cast<char>(menu.size() - lengthPos - 1);

		return DHCPOption(static_cast<_BYTE>(PXEOptions::BootMenue), menu);
	}

	DHCPOption PxeRBCP::GenerateBootMenuePrompt() const
	{
		std::vector<char> prompt;

		// Menu Prompt (79)
		prompt.push_back(PXEOptions::MenuPrompt);

		// Build prompt: timeout + text
		std::string text = menueTimeout == 0xFF ? 
			menuePrompt.front() : 
			menuePrompt.back();

		// Length = 1 (timeout) + text length
		prompt.push_back(static_cast<char>(1 + text.length()));

		// Timeout byte
		prompt.push_back(static_cast<char>(menueTimeout));

		// Prompt text
		prompt.insert(prompt.end(), text.begin(), text.end());
		prompt.push_back(0);  // Null terminator

		return DHCPOption(static_cast<_BYTE>(PXEOptions::MenuPrompt), prompt);
	}

	_STRING PxeRBCP::GetServerTypeName(BootServerType type) const
	{
		switch (type)
		{
		case BootServerType::BIOSBoot: return "BIOS Boot";
		case BootServerType::NTLDR: return "NTLDR";
		case BootServerType::DOS: return "DOS";
		case BootServerType::BCD: return "BCD";
		case BootServerType::UEFIARM: return "UEFI ARM";
		case BootServerType::UEFIApple: return "UEFI Apple";
		case BootServerType::PXELINUX: return "PXELINUX";
		case BootServerType::BISConfig: return "BIS";
		case BootServerType::WDSNBP: return "WDS NBP";
		case BootServerType::PXEBootstrapServer: return "PXE Client";
		case BootServerType::HTTPClient: return "HTTP Client";
		case BootServerType::UEFIHTTPBoot: return "UEFI HTTP Boot";
		case BootServerType::iSCSI: return "iSCSI";
		case BootServerType::FCoE: return "FCoE";
		default: return "Unknown";
		}
	}
}
