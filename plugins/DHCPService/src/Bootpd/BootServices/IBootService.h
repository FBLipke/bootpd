#pragma once
#include "../../Bootpd.h"
#include "../Network/Packet/DHCPPacket.h"
#include "../Defines/Definitions.h"

namespace bootp::Plugins::DHCP::BootServices
{
	/**
	 * @brief Boot server type enumeration - matches WDS behavior IDs
	 */
	enum class BootServerType : _WORD
	{
		None = 0,

		// Standard Boot Server Types (WDS aligned)
		BIOSBoot = 0,            // behavior="0" - BIOS Boot
		NTLDR = 1,              // behavior="1" - x86 NTLDR/OSChooser  
		DOS = 3,                // behavior="3" - DOS
		BCD = 7,                // behavior="7" - BCD boot

		// Extended Types (WDS aligned)
		UEFIARM = 65530,        // behavior="65530" - ARM UEFI
		UEFIApple = 65531,      // behavior="65531" - Apple EFI
		PXELINUX = 65532,       // behavior="65532" - PXELINUX
		BISConfig = 65533,      // behavior="65533" - BIS Config
		WDSNBP = 65534,         // behavior="65534" - WDS Network Bootstrap Program
		APITest = 65535,        // behavior="65535" - API Test

		// Custom Types
		PXEClient = 100,          // DHCP Option 60 PXEClient
		HTTPClient = 101,        // HTTP/iSCSI Boot Client
		UEFIHTTPBoot = 102,      // UEFI HTTP Boot
		iSCSI = 103,             // iSCSI Boot
		FCoE = 104,              // Fibre Channel over Ethernet
	};

	/**
	 * @brief Base interface for all Boot Service plugins
	 * 
	 * Each boot service handles a specific type of network boot request.
	 * Services are registered at startup and called via the registry
	 * when DHCP requests come in.
	 */
	class IBootService
	{
	public:
		virtual ~IBootService() = default;

		/**
		 * @brief Get the boot server type this service handles
		 */
		virtual BootServerType GetServerType() const = 0;

		/**
		 * @brief Get the human-readable name of this boot service
		 */
		virtual _STRING GetName() const = 0;

		/**
		 * @brief Check if this service can handle the given request
		 * @param request The DHCP packet to evaluate
		 * @return true if this service should handle the request
		 */
		virtual bool CanHandle(const DHCPPacket& request) const = 0;

		/**
		 * @brief Handle a DHCP Discover request
		 * @param server Server identifier
		 * @param socket Socket identifier
		 * @param client Client identifier
		 * @param request The DHCP discover packet
		 * @return true if response was sent
		 */
		virtual bool OnDiscover(const _STRING& server, const _STRING& socket,
							   const _STRING& client, const DHCPPacket& request) = 0;

		/**
		 * @brief Handle a DHCP Request (renewal) request
		 * @param server Server identifier
		 * @param socket Socket identifier
		 * @param client Client identifier
		 * @param request The DHCP request packet
		 * @return true if response was sent
		 */
		virtual bool OnRequest(const _STRING& server, const _STRING& socket,
							 const _STRING& client, const DHCPPacket& request) = 0;

		/**
		 * @brief Handle a DHCP Release request
		 * @param server Server identifier
		 * @param socket Socket identifier
		 * @param client Client identifier
		 * @param request The DHCP release packet
		 */
		virtual void OnRelease(const _STRING& server, const _STRING& socket,
							  const _STRING& client, const DHCPPacket& request) = 0;

		/**
		 * @brief Handle a DHCP Inform request
		 * @param server Server identifier
		 * @param socket Socket identifier
		 * @param client Client identifier
		 * @param request The DHCP inform packet
		 */
		virtual void OnInform(const _STRING& server, const _STRING& socket,
							 const _STRING& client, const DHCPPacket& request) = 0;
	};

	/**
	 * @brief Registry for all registered Boot Service plugins
	 * 
	 * Singleton pattern - manages all IBootService implementations
	 * and dispatches requests to the appropriate service based on
	 * CanHandle() evaluation.
	 */
	class BootServiceRegistry
	{
	public:
		static BootServiceRegistry& Instance();

		/**
		 * @brief Register a boot service
		 * @param service Pointer to the boot service (ownership transferred)
		 * @return true if registered successfully
		 */
		bool Register(std::unique_ptr<IBootService> service);

		/**
		 * @brief Unregister a boot service by type
		 */
		void Unregister(BootServerType type);

		/**
		 * @brief Find the appropriate boot service for a request
		 * @param request The DHCP packet to evaluate
		 * @return Pointer to the service, or nullptr if none matches
		 */
		IBootService* FindService(const DHCPPacket& request) const;

		/**
		 * @brief Get a service by type
		 * @return Pointer to the service, or nullptr if not found
		 */
		IBootService* GetService(BootServerType type) const;

		/**
		 * @brief Get all registered services
		 */
		const std::map<BootServerType, std::unique_ptr<IBootService>>& GetAllServices() const;

		/**
		 * @brief Clear all registered services
		 */
		void Clear();

	private:
		BootServiceRegistry() = default;
		~BootServiceRegistry() = default;

		BootServiceRegistry(const BootServiceRegistry&) = delete;
		BootServiceRegistry& operator=(const BootServiceRegistry&) = delete;

		std::map<BootServerType, std::unique_ptr<IBootService>> services;
	};

	/**
	 * @brief Helper macro to register a boot service plugin
	 * 
	 * Usage in plugin's entry point:
	 * REGISTER_BOOT_SERVICE(MyPXEBootService);
	 */
	#define REGISTER_BOOT_SERVICE(ServiceClass) \
		extern "C" \
		{ \
			__declspec(dllexport) bool RegisterBootService() \
			{ \
				return BootServiceRegistry::Instance().Register( \
					std::make_unique<ServiceClass>()); \
			} \
		}
}
