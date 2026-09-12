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

#pragma once
#include "../Bootpd.h"
#include <cstdint>

namespace bootp
{
	namespace Network
	{
		class Socket :
			public ISocket
		{
		public:
			Socket(const uint16_t& port);
			~Socket();

			void ReceiveFrom(const Socket* socket);
			void Init() override;
			void Start() override;
			void Listen() override;
			void HeartBeat() override;
			void Close() override;
		private:
			sockaddr_in _local;
			sockaddr_in _remote;
			SOCKET_T _sock;
			uint16_t port;
		};
	}
}
