/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Bootpd.h"

namespace bootp
{
	// Static member - über IBootpd Interface zugänglich
	__LIBEXPORT std::map<_STRING, std::unique_ptr<IBootpd>> IBootpd::_subSystems;

	bootpd::bootpd()
	{
	}

	bootpd::~bootpd()
	{
	}

	void bootpd::Add_Subsys(const _STRING &str, std::unique_ptr<IBootpd> subsys)
	{
		IBootpd::_subSystems.emplace(str, std::move(subsys));
	}

	IBootpd *bootpd::Get_SubSystem(const _STRING &id)
	{
		return IBootpd::_subSystems.at(id).get();
	}

	_BOOL bootpd::Init(const _INT32 &argc, const char *argv[])
	{
		this->Add_Subsys("ServiceManager", std::make_unique<bootp::Services::ServiceManager>());
		this->Add_Subsys("ServerManager", std::make_unique<bootp::Network::ServerManager>());
		this->Add_Subsys("ClientManager", std::make_unique<bootp::Network::ClientManager>());

		// ERST: Alle Init() aufrufen
		for (const auto &s : IBootpd::_subSystems)
		{
			s.second.get()->Init(argc, argv);
		}

		// DANN: ServerManager.HandleManager_Request auf ServiceManager.HandleManager_Request verbinden
		auto *clientMgr = static_cast<bootp::Network::ClientManager *>(
			IBootpd::_subSystems["ClientManager"].get());
		auto *serverMgr = static_cast<bootp::Network::ServerManager *>(
			IBootpd::_subSystems["ServerManager"].get());
		auto *serviceMgr = static_cast<bootp::Services::ServiceManager *>(
			IBootpd::_subSystems["ServiceManager"].get());

		// ServerManager leitet Daten an ServiceManager weiter
		serverMgr->Handle_Manager_Request =
			[clientMgr, serviceMgr](const _STRING &server_id, const _STRING &socket_id, const std::shared_ptr<bootp::Network::IPacket> &request, const _IPADDR &ip, const _USHORT &port, const _STRING &id)
		{
			if (clientMgr->Add)
			{
				_STRING clientId = clientMgr->Add(id, ip, port);
				if (serviceMgr->Handle_Manager_Request)
				{
					serviceMgr->Handle_Manager_Request(server_id, socket_id, request, ip, port, id);
				}
			}
		};

		return true;
	}

	void bootpd::Add_Server(const std::vector<_USHORT> &ports)
	{
	}

	void bootpd::HeartBeat()
	{
		for (const auto &s : IBootpd::_subSystems)
		{
			s.second.get()->HeartBeat();
		}
	}

	_BOOL bootpd::Start()
	{
		for (const auto &s : IBootpd::_subSystems)
		{
			s.second.get()->Start();
		}

		return true;
	}

	void bootpd::Close()
	{
		for (const auto &s : IBootpd::_subSystems)
		{
			s.second.get()->Close();
		}
	}
}
