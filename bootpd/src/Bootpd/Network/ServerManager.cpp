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

namespace bootp
{
	namespace Network
	{
#ifdef _WIN32
		_BOOL ServerManager::Init_Winsock(_INT32 major, _INT32 minor)
		{
			ClearBuffer(&wsa, sizeof wsa);
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

		_BOOL ServerManager::Init(const _INT32 &argc, const _BYTE *argv[])
		{
			printf("[D] Bootpd - ServerMgr...\n");
#ifdef WIN32
			Init_Winsock(2, 0);
#endif
			auto _id = Functions::GenerateUUID();
			this->servers.emplace(_id, std::make_unique<bootp::Network::Server>(_id));

			for (const auto &s : this->servers)
			{
				auto *srv = s.second.get();

				// ServerDataReceived ruft HandleManager_Request auf
				srv->ServerDataReceived = [this](const _STRING &server_id, const _STRING &socket_id, const _BYTE *buffer, const _SIZET &length)
				{
					if (this->HandleManager_Request)
						this->HandleManager_Request(server_id, socket_id, buffer, length);
				};

				srv->Init();
			}

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
}
