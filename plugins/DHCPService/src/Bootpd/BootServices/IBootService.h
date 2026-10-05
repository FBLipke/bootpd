#pragma once
#include "../Bootpd.h"
#include "../Defines/Definitions.h"

// Forward declare DHCPPacket to avoid circular dependency
namespace bootp::Plugins::DHCP::Network::Packet
{
	class DHCPPacket;
}

namespace bootp::Plugins::DHCP
{
	/**
	 * @brief Boot server type enumeration - matches Netbootd RFC 4578
	 */
	enum class BootServerType : _USHORT
	{
		PXEBootstrapServer = 0,			  // behavior="0" - Default PXE Bootstrap
		MicrosoftWindowsNT = 1,			  // behavior="1" - RIS/WSUS
		IntelLCM = 2,					  // Intel LCM
		DOSUNDI = 3,					  // behavior="3" - DOS/Undi
		NECESMPRO = 4,					  // NEC ESMPRO
		IBMWSoD = 5,					  // IBM WS-oD
		IBMLCCM = 6,					  // IBM LCCM
		CAUnicenterTNG = 7,				  // CA Unicenter TNG
		HPOpenView = 8,					  // HP OpenView
		Reserved = 9,					  // Reserved
		Vendor = 32768,					  // Vendor-specific (0x8000)
		AppleLegacy = 0xFFFA,			  // Apple Legacy (ushort.MaxValue - 5)
		AppleBootServer = 0xFFFB,		  // Apple Boot Server (ushort.MaxValue - 4)
		LinuxBootServer = 0xFFFC,		  // Linux Boot Server (ushort.MaxValue - 3)
		BootIntegrityService = 0xFFFD,	  // BIS (ushort.MaxValue - 2)
		WindowsDeploymentServer = 0xFFFE, // WDS (ushort.MaxValue - 1)
		ApiTest = 0xFFFF,				  // API Test (ushort.MaxValue)
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
		virtual bool CanHandle(const std::shared_ptr<Network::Packet::DHCPPacket> &request) const = 0;

		/**
		 * @brief Handle a DHCP Discover request
		 * @param server Server identifier
		 * @param socket Socket identifier
		 * @param client Client identifier
		 * @param request The DHCP discover packet
		 * @return true if response was sent
		 */
		virtual bool OnDiscover(const _STRING &server, const _STRING &socket,
							   const _STRING &client, const std::shared_ptr<Network::Packet::DHCPPacket> &request) = 0;

		/**
		 * @brief Handle a DHCP Request (renewal) request
		 * @param server Server identifier
		 * @param socket Socket identifier
		 * @param client Client identifier
		 * @param request The DHCP request packet
		 * @return true if response was sent
		 */
		virtual bool OnRequest(const _STRING &server, const _STRING &socket,
							  const _STRING &client, const std::shared_ptr<Network::Packet::DHCPPacket> &request) = 0;

		/**
		 * @brief Handle a DHCP Release request
		 * @param server Server identifier
		 * @param socket Socket identifier
		 * @param client Client identifier
		 * @param request The DHCP release packet
		 */
		virtual void OnRelease(const _STRING &server, const _STRING &socket,
							   const _STRING &client, const std::shared_ptr<Network::Packet::DHCPPacket> &request) = 0;

		/**
		 * @brief Handle a DHCP Inform request
		 * @param server Server identifier
		 * @param socket Socket identifier
		 * @param client Client identifier
		 * @param request The DHCP inform packet
		 */
		virtual void OnInform(const _STRING &server, const _STRING &socket,
							 const _STRING &client, const std::shared_ptr<Network::Packet::DHCPPacket> &request) = 0;
	};

	/**
	 * @brief Registry for all registered Boot Service plugins
	 *
	 * Singleton pattern - manages all IBootService implementations
	 * and dispatches requests to the appropriate service based on
	 * CanHandle() evaluation.
	 *
	 * Supports MULTIPLE services per BootServerType (fallback chain)
	 * matching the Netbootd pattern.
	 */
	class BootServiceRegistry
	{
	public:
		static BootServiceRegistry &Instance();

		/**
		 * @brief Register a boot service
		 * @param service Pointer to the boot service (ownership transferred)
		 * @return true if registered successfully
		 */
		bool Register(std::unique_ptr<IBootService> service);

		/**
		 * @brief Unregister a boot service by type (removes first match)
		 */
		void Unregister(BootServerType type);

		/**
		 * @brief Find the appropriate boot service for a request
		 * Tries each service in order until CanHandle() returns true
		 * @param request The DHCP packet to evaluate
		 * @return Pointer to the service, or nullptr if none matches
		 */
		IBootService *FindService(const std::shared_ptr<Network::Packet::DHCPPacket> &request) const;

		/**
		 * @brief Get all services for a specific type
		 * @return Vector of services (empty if none registered)
		 */
		std::vector<IBootService *> GetServices(BootServerType type) const;

		/**
		 * @brief Get a service by type and index
		 * @return Pointer to the service, or nullptr if not found
		 */
		IBootService *GetService(BootServerType type, size_t index = 0) const;

		/**
		 * @brief Get all registered services
		 */
		const std::map<BootServerType, std::vector<std::unique_ptr<IBootService>>> &
		GetAllServices() const;

		/**
		 * @brief Clear all registered services
		 */
		void Clear();

		/**
		 * @brief Get count of registered services
		 */
		size_t Count() const;

	private:
		BootServiceRegistry() = default;
		~BootServiceRegistry() = default;

		BootServiceRegistry(const BootServiceRegistry &) = delete;
		BootServiceRegistry &operator=(const BootServiceRegistry &) = delete;

		// Multiple services per type (allows fallback chaining)
		std::map<BootServerType, std::vector<std::unique_ptr<IBootService>>> services;
	};

/**
 * @brief Helper macro to register a boot service plugin
 *
 * Usage in plugin's entry point:
 * REGISTER_BOOT_SERVICE(MyPXEBootService);
 */
#define REGISTER_BOOT_SERVICE(ServiceClass)                  \
	extern "C"                                               \
	{                                                        \
		__declspec(dllexport) bool RegisterBootService()     \
		{                                                    \
			return BootServiceRegistry::Instance().Register( \
				std::make_unique<ServiceClass>());           \
		}                                                    \
	}
}
