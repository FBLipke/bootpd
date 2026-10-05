#include "IBootService.h"

namespace bootp::Plugins::DHCP::BootServices
{
	BootServiceRegistry& BootServiceRegistry::Instance()
	{
		static BootServiceRegistry instance;
		return instance;
	}

	bool BootServiceRegistry::Register(std::unique_ptr<IBootService> service)
	{
		if (!service)
			return false;

		auto type = service->GetServerType();
		
		// Check if already registered
		if (services.find(type) != services.end())
		{
			// Replace existing service
			services[type] = std::move(service);
		}
		else
		{
			services.emplace(type, std::move(service));
		}

		return true;
	}

	void BootServiceRegistry::Unregister(BootServerType type)
	{
		services.erase(type);
	}

	IBootService* BootServiceRegistry::FindService(const DHCPPacket& request) const
	{
		for (const auto& [type, service] : services)
		{
			if (service->CanHandle(request))
			{
				return service.get();
			}
		}
		return nullptr;
	}

	IBootService* BootServiceRegistry::GetService(BootServerType type) const
	{
		auto it = services.find(type);
		if (it != services.end())
		{
			return it->second.get();
		}
		return nullptr;
	}

	const std::map<BootServerType, std::unique_ptr<IBootService>>& 
	BootServiceRegistry::GetAllServices() const
	{
		return services;
	}

	void BootServiceRegistry::Clear()
	{
		services.clear();
	}
}
