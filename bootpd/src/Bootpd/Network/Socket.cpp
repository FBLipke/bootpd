#include "Socket.h"
#include <cstring>
#include <unistd.h>

namespace bootp
{
	namespace Network
	{
		void Socket::Init()
		{
			int yes = 1;
			int no = 0;
			int val_length = sizeof(int);

			printf("[D] Socket -> Init(%u)\n", htons(this->port));

			this->_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			memset(&this->_local, 0, sizeof this->_local);
			this->_local.sin_addr.s_addr = INADDR_ANY;
			this->_local.sin_port = this->port;
			this->_local.sin_family = AF_INET;

			auto retval = setsockopt(this->_sock, SOL_SOCKET, SO_BROADCAST, (char*)&yes, val_length);
			if (retval == SOCKET_ERROR)
				return;

			retval = setsockopt(this->_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&no, val_length);
			if (retval == SOCKET_ERROR)
				return;
		}

		void Socket::Start()
		{
			printf("[D] Socket -> Start()\n");
			auto retval = bind(this->_sock, reinterpret_cast<struct sockaddr*>(&this->_local), sizeof this->_local);
			if (retval == SOCKET_ERROR)
			{
				printf("[E] bind: Cannot bind on Interface\n");
			}
		}

		void Socket::Listen()
		{
			printf("[D] Socket -> Listen()\n");
			std::thread t([this]() { this->ReceiveFrom(this); });
			t.detach();
		}

		Socket::Socket(const uint16_t& port)
		{
			this->port = htons(port);
		}

		Socket::~Socket()
		{
		}

		void Socket::ReceiveFrom(const Socket* socket)
		{
			socklen_t hostAddrSize = 0;
			char tempBuffer[65536];

			while (true)
			{
				hostAddrSize = sizeof(sockaddr_in);

				int messageLength = recvfrom(socket->_sock, tempBuffer,
					65536, 0, (sockaddr*)&socket->_remote, &hostAddrSize);

				if (messageLength <= 0)
					break;

				tempBuffer[messageLength] = '\0';

				if (socket->SocketDataReceived)
					socket->SocketDataReceived(tempBuffer, messageLength);
			}
		}

		void Socket::HeartBeat()
		{
			printf("[D] Socket -> HeartBeat()\n");
		}

		void Socket::Close()
		{
			printf("[D] Socket -> Close()\n");
		}
	}
}