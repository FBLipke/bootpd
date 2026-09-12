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
#pragma once

namespace bootp
{
	namespace Network
	{
		void Server::Init()
		{
			std::vector<uint16_t> ports;
			ports.emplace_back(67);
			ports.emplace_back(4011);
						
			printf("[D] Server -> Init()\n");
			for (const auto& port : ports)
			{
				this->sockets.emplace_back(std::make_unique<bootp::Network::Socket>(port));
			}

			for (const auto& s : this->sockets) {
				s.get()->Init();

			}
		}

		void Server::Start()
		{
			printf("[D] Server -> Start()\n");

			for (const auto& s : this->sockets) {
				s.get()->SocketDataReceived = [&](const char* buffer, const size_t& length) {
					this->ServerDataReceived(buffer, length);
					};
				s.get()->Start();
			}
		}

		void Server::Listen()
		{
			printf("[D] Server -> Listen()\n");
			for (const auto& s : this->sockets) {
				s.get()->Listen();
			}
		}

		void Server::HeartBeat()
		{
			printf("[D] Server -> HeartBeat()\n");
			for (const auto& s : this->sockets) {
				s.get()->HeartBeat();
			}
		}

		void Server::Close()
		{
			printf("[D] Server -> Close()\n");
			for (const auto& s : this->sockets) {
				s.get()->Close();
			}
		}
	}
}