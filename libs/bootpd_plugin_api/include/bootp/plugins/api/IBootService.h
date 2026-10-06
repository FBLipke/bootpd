#pragma once
#include "Types.h"

namespace bootp::plugins::api
{
	/**
	 * @brief Boot server type enumeration - RFC 4578 / WDS NBS
	 */
	enum class BootServerType : uint16
	{
		PXEBootstrapServer = 0,           // Default PXE Bootstrap
		MicrosoftWindowsNT = 1,           // RIS/WSUS
		IntelLCM = 2,                    // Intel LCM
		DOSUNDI = 3,                    // DOS/Undi
		NECESMPRO = 4,                  // NEC ESMPRO
		IBMWSoD = 5,                    // IBM WS-oD
		IBMLCCM = 6,                    // IBM LCCM
		CAUnicenterTNG = 7,             // CA Unicenter TNG
		HPOpenView = 8,                 // HP OpenView
		Reserved = 9,                   // Reserved
		Vendor = 32768,                 // Vendor-specific (0x8000)
		AppleLegacy = 0xFFFA,          // Apple Legacy
		AppleBootServer = 0xFFFB,       // Apple Boot Server
		LinuxBootServer = 0xFFFC,      // Linux Boot Server
		BootIntegrityService = 0xFFFD,  // BIS
		WindowsDeploymentServer = 0xFFFE, // WDS
		ApiTest = 0xFFFF,              // API Test
	};

	/**
	 * @brief Minimal packet interface for plugin API
	 * 
	 * This is a minimal interface that DHCPPacket implements.
	 * Plugins work with this interface to avoid network library dependency.
	 */
	class IPacket
	{
	public:
		virtual ~IPacket() = default;
		virtual const uint8* GetData() const = 0;
		virtual size GetSize() const = 0;
		virtual uint32 GetTransactionID() const = 0;
	};

	/**
	 * @brief Base interface for all Boot Service plugins
	 * 
	 * Each boot service handles a specific type of network boot request.
	 * Services are registered at startup and called via the registry.
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
		virtual string GetName() const = 0;

		/**
		 * @brief Check if this service can handle the given request
		 */
		virtual bool CanHandle(const shared_ptr<IPacket>& request) const = 0;

		/**
		 * @brief Handle a DHCP Discover request
		 */
		virtual bool OnDiscover(const string& server, const string& socket,
							   const string& client, const shared_ptr<IPacket>& request) = 0;

		/**
		 * @brief Handle a DHCP Request (renewal) request
		 */
		virtual bool OnRequest(const string& server, const string& socket,
							  const string& client, const shared_ptr<IPacket>& request) = 0;

		/**
		 * @brief Handle a DHCP Release request
		 */
		virtual void OnRelease(const string& server, const string& socket,
							   const string& client, const shared_ptr<IPacket>& request) = 0;

		/**
		 * @brief Handle a DHCP Inform request
		 */
		virtual void OnInform(const string& server, const string& socket,
							 const string& client, const shared_ptr<IPacket>& request) = 0;
	};

	/**
	 * @brief Registry for all registered Boot Service plugins
	 * 
	 * Singleton pattern - manages all IBootService implementations.
	 */
	class BootServiceRegistry
	{
	public:
		static BootServiceRegistry& Instance();

		/**
		 * @brief Register a boot service
		 */
		virtual bool Register(shared_ptr<IBootService> service) = 0;

		/**
		 * @brief Unregister a boot service by type
		 */
		virtual void Unregister(BootServerType type) = 0;

		/**
		 * @brief Find the appropriate boot service for a request
		 */
		virtual IBootService* FindService(const shared_ptr<IPacket>& request) const = 0;

		/**
		 * @brief Get all services for a specific type
		 */
		virtual vector<IBootService*> GetServices(BootServerType type) const = 0;

		/**
		 * @brief Get a service by type and index
		 */
		virtual IBootService* GetService(BootServerType type, size index = 0) const = 0;

		/**
		 * @brief Clear all registered services
		 */
		virtual void Clear() = 0;

		/**
		 * @brief Get count of registered services
		 */
		virtual size Count() const = 0;
	};

	/**
	 * @brief Plugin entry point - must be exported by each plugin DLL
	 */
	extern "C"
	{
		// Plugins must implement this function
		typedef bool (*RegisterBootServiceFn)();
	}
}
