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
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include <chrono>
#include <thread>
#include <string>

#ifdef _WIN32
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
typedef int SOCKET_T;
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
typedef int SOCKET_T;
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#define SOCKADDR_IN sockaddr_in
#endif

class IBootpd
{
public:
	virtual bool Init(const int argc, const char* argv[]) = 0;
	virtual bool Start() = 0;
	virtual void HeartBeat() = 0;
	virtual void Close() = 0;
};
#include "Common/Functions.h"
#include "Network/ISocket.h"
#include "Network/Socket.h"
#include "Network/IServer.h"
#include "Network/Server.h"
#include "Network/ServerManager.h"

namespace bootp
{
	class bootpd : public IBootpd
	{
	public:
		bootpd();
		~bootpd();

		bool Init(const int argc, const char* argv[]) override;

		bool Start() override;

		void HeartBeat() override;

		void Close() override;
	private:
		std::vector<std::unique_ptr<IBootpd>> _subSystems;
	};
}
