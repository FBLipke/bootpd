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

#include "Server.h"

namespace bootp
{
	namespace Network
	{
		Server::Server(const _STRING &id)
		{
			this->id = id;
		}

		Server::~Server()
		{
		}

		void Server::Init()
		{

			_IPADDR address = 0;
			std::vector<_IPADDR> addresses;

			std::vector<_USHORT> ports;
			ports.emplace_back(67);
			ports.emplace_back(4011);

			_USHORT index = 0;
			printf("[D] Server[%s] -> Init()\n", this->id.c_str());
#ifdef _WIN32
			PIP_ADAPTER_INFO pAdapterInfo = nullptr;
			PIP_ADAPTER_INFO pAdapter = nullptr;
			_ULONG ulOutBufLen = 0;

			if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
			{
				free(pAdapterInfo);
				pAdapterInfo = static_cast<IP_ADAPTER_INFO *>(malloc(ulOutBufLen));
				if (pAdapterInfo == nullptr)
					return;
			}

			if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == NO_ERROR)
			{
				pAdapter = pAdapterInfo;
				while (pAdapter)
				{
					address = inet_addr(pAdapter->IpAddressList.IpAddress.String);
					if (address == inet_addr("127.0.0.1") || address == 0)
					{
						pAdapter = pAdapter->Next;
						continue;
					}

					addresses.emplace_back(address);

					for (const auto &port : ports)
					{
						auto _id = Functions::GenerateUUID();

						this->sockets.emplace_back(
							std::make_unique<bootp::Network::Socket>(_id, address, port));
					}

					pAdapter = pAdapter->Next;
				}

				pAdapter = nullptr;
			}

			if (pAdapterInfo)
				free(pAdapterInfo);

			pAdapterInfo = nullptr;
#else
			ifaddrs *ifap = nullptr, *ifa = nullptr;
			getifaddrs(&ifap);

			for (ifa = ifap; ifa; ifa = ifa->ifa_next)
			{
				if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET)
				{
					address = ((struct sockaddr_in *)ifa->ifa_addr)->sin_addr.s_addr;

					if (address == inet_addr("127.0.0.1") || address == 0)
						continue;

					addresses.emplace_back(address);

					for (const auto &port : ports)
					{
						auto _id = Functions::GenerateUUID();

						this->sockets.emplace_back(
							std::make_unique<bootp::Network::Socket>(_id, address, port));
					}
				}
			}

			freeifaddrs(ifap);
			ifap = nullptr;
#endif

			for (const auto &s : this->sockets)
			{
				s.get()->Init(AF_INET);
			}
		}

		void Server::Start()
		{
			printf("[D] Server[%s] -> Start()\n", this->id.c_str());

			for (const auto &s : this->sockets)
			{
				s.get()->SocketDataReceived = [&](const _STRING &socket_id, const _BYTE *buffer, const _SIZET &length)
				{
					this->ServerDataReceived(this->id, socket_id, buffer, length);
				};
				s.get()->Start();
			}
		}

		void Server::Listen()
		{
			printf("[D] Server[%s] -> Listen()\n", this->id.c_str());
			for (const auto &s : this->sockets)
			{
				s.get()->Listen();
			}
		}

		void Server::HeartBeat()
		{
			for (const auto &s : this->sockets)
			{
				s.get()->HeartBeat();
			}
		}

		void Server::Close()
		{
			printf("[D] Server[%s] -> Close()\n", this->id.c_str());
			for (const auto &s : this->sockets)
			{
				s.get()->Close();
			}
		}
	}
}