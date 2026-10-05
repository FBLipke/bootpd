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
		services[type].push_back(std::move(service));
		return true;
	}

	void BootServiceRegistry::Unregister(BootServerType type)
	{
		auto it = services.find(type);
		if (it != services.end() && !it->second.empty())
		{
			it->second.erase(it->second.begin()); // Remove first registered
		}
	}

	IBootService* BootServiceRegistry::FindService(const Network::Packet::DHCPPacket& request) const
	{
		for (const auto& [type, serviceList] : services)
		{
			for (const auto& service : serviceList)
			{
				if (service->CanHandle(request))
				{
					return service.get();
				}
			}
		}
		return nullptr;
	}

	std::vector<IBootService*> BootServiceRegistry::GetServices(BootServerType type) const
	{
		std::vector<IBootService*> result;
		auto it = services.find(type);
		if (it != services.end())
		{
			for (const auto& svc : it->second)
			{
				result.push_back(svc.get());
			}
		}
		return result;
	}

	IBootService* BootServiceRegistry::GetService(BootServerType type, size_t index) const
	{
		auto it = services.find(type);
		if (it != services.end() && index < it->second.size())
		{
			return it->second[index].get();
		}
		return nullptr;
	}

	const std::map<BootServerType, std::vector<std::unique_ptr<IBootService>>>&
	BootServiceRegistry::GetAllServices() const
	{
		return services;
	}

	void BootServiceRegistry::Clear()
	{
		services.clear();
	}

	size_t BootServiceRegistry::Count() const
	{
		size_t total = 0;
		for (const auto& [type, list] : services)
		{
			total += list.size();
		}
		return total;
	}
}
