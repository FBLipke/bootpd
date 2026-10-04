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

#include "ServerManager.h"

namespace bootp::Network
{
#ifdef _WIN32
	_BOOL ServerManager::Init_Winsock(_INT32 major, _INT32 minor)
	{
		_ClearBuffer(&wsa, sizeof wsa);
		return WSAStartup(MAKEWORD(major, minor), &wsa) == 0;
	}

	_BOOL ServerManager::Close_Winsock()
	{
		return WSACleanup() == 0;
	}
#endif

	ServerManager::ServerManager()
	{
	}

	ServerManager::~ServerManager()
	{
	}

	void ServerManager::Add_Server(const std::vector<_USHORT> &ports)
	{
		auto _id = Functions::GenerateUUID();
		this->servers.emplace(_id, std::make_unique<bootp::Network::Server>(_id));

		for (const auto &s : this->servers)
		{
			auto *srv = s.second.get();

			// ServerDataReceived ruft HandleManager_Request auf
			srv->ServerDataReceived = [this](const _STRING &server_id, const _STRING &socket_id, const std::shared_ptr<IPacket> &request, const _IPADDR &ip, const _USHORT &port, const _STRING &client)
			{
				if (this->Handle_Manager_Request)
					this->Handle_Manager_Request(server_id, socket_id, request, ip, port, client);
			};

			srv->Init(ports);
			srv->Start();
			srv->Listen();
		}
	}

	_BOOL ServerManager::Init(IBootpd *, const _INT32 &argc, const char *argv[])
	{
		printf("[D] Bootpd - ServerMgr...\n");
#ifdef WIN32
		Init_Winsock(2, 0);
#endif

		return true;
	}

	_BOOL ServerManager::Start()
	{
		for (const auto &s : this->servers)
		{
			s.second.get()->Start();
			s.second.get()->Listen();
		}

		return true;
	}

	void ServerManager::HeartBeat()
	{
		for (const auto &s : this->servers)
			s.second.get()->HeartBeat();
	}

	IBootpd *ServerManager::Get_SubSystem(const _STRING &id)
	{
		return nullptr;
	}

	void ServerManager::Close()
	{
		for (const auto &s : this->servers)
			s.second.get()->Close();

		this->servers.clear();
#ifdef WIN32
		Close_Winsock();
#endif
	}
}
