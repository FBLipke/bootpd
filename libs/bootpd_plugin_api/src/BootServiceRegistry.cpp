#include "bootp/plugins/api/IBootService.h"
#include <map>
#include <mutex>

namespace bootp::plugins::api
{
	class BootServiceRegistryImpl : public BootServiceRegistry
	{
	public:
		static BootServiceRegistryImpl& Instance()
		{
			static BootServiceRegistryImpl instance;
			return instance;
		}

		bool Register(shared_ptr<IBootService> service) override
		{
			if (!service) return false;
			
			std::lock_guard<std::mutex> lock(mutex_);
			BootServerType type = service->GetServerType();
			services_[type].push_back(std::move(service));
			return true;
		}

		void Unregister(BootServerType type) override
		{
			std::lock_guard<std::mutex> lock(mutex_);
			services_.erase(type);
		}

		IBootService* FindService(const shared_ptr<IPacket>& request) const override
		{
			std::lock_guard<std::mutex> lock(mutex_);
			
			for (auto& [type, serviceList] : services_)
			{
				for (auto& service : serviceList)
				{
					if (service->CanHandle(request))
					{
						return service.get();
					}
				}
			}
			return nullptr;
		}

		vector<IBootService*> GetServices(BootServerType type) const override
		{
			std::lock_guard<std::mutex> lock(mutex_);
			vector<IBootService*> result;
			
			auto it = services_.find(type);
			if (it != services_.end())
			{
				for (auto& service : it->second)
				{
					result.push_back(service.get());
				}
			}
			return result;
		}

		IBootService* GetService(BootServerType type, size index) const override
		{
			std::lock_guard<std::mutex> lock(mutex_);
			
			auto it = services_.find(type);
			if (it != services_.end() && index < it->second.size())
			{
				return it->second[index].get();
			}
			return nullptr;
		}

		void Clear() override
		{
			std::lock_guard<std::mutex> lock(mutex_);
			services_.clear();
		}

		size Count() const override
		{
			std::lock_guard<std::mutex> lock(mutex_);
			size total = 0;
			for (auto& [type, list] : services_)
			{
				total += list.size();
			}
			return total;
		}

	private:
		BootServiceRegistryImpl() = default;
		~BootServiceRegistryImpl() = default;

		BootServiceRegistryImpl(const BootServiceRegistryImpl&) = delete;
		BootServiceRegistryImpl& operator=(const BootServiceRegistryImpl&) = delete;

		mutable std::mutex mutex_;
		std::map<BootServerType, std::vector<shared_ptr<IBootService>>> services_;
	};

	BootServiceRegistry& BootServiceRegistry::Instance()
	{
		return BootServiceRegistryImpl::Instance();
	}
}
