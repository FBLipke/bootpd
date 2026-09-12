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
#ifdef _WIN32
	bool ServerManager::Init_Winsock(int major, int minor)
	{
		memset(&wsa, 0, sizeof wsa);

		return WSAStartup(MAKEWORD(major, minor), &wsa) == 0;
	}

	bool ServerManager::Close_Winsock()
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

	bool ServerManager::Init(const int argc, const char* argv[])
	{
		printf("[D] Bootpd - ServerMgr...\n");
#ifdef WIN32
		Init_Winsock(2, 0);
#endif
		this->servers.emplace_back(std::make_unique<bootp::Network::Server>());

		for (const auto& s : this->servers) {
			s.get()->ServerDataReceived = [&](const char* buffer, const size_t& length) {
				printf("Got packet!\n");
				};

			s.get()->Init();
		}
	
		return true;
	}

	bool ServerManager::Start()
	{
		for (const auto& s : this->servers) {
			s.get()->Start();
			s.get()->Listen();
		}

		return true;
	}

	void ServerManager::HeartBeat()
	{
		for (const auto& s : this->servers) {
			s.get()->HeartBeat();
		}
	}

	void ServerManager::Close()
	{
		for (const auto& s : this->servers) {
			s.get()->Close();
		}

#ifdef WIN32
		Close_Winsock();
#endif
	}
}
