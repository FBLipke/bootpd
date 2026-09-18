#include "Socket.h"

namespace bootp
{
	namespace Network
	{
		void Socket::Init(const _INT32 &af)
		{
			int yes = 1;
			int no = 0;
			int val_length = sizeof(int);

			this->proto = proto;

			printf("[D] Socket[%s] -> Init(%u)\n",
				   this->id.c_str(), htons(this->port));

			this->socketType = SOCK_DGRAM;

			this->_sock = socket(af, this->socketType, this->proto);
			memset(&this->_local, 0, sizeof this->_local);
			this->_local.sin_addr.s_addr = this->address;
			this->_local.sin_port = this->port;
			this->_local.sin_family = af;

			auto retval = setsockopt(this->_sock, SOL_SOCKET, SO_BROADCAST, (char *)&yes, val_length);
			// retval = setsockopt(this->_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&yes, val_length);
		}

		void Socket::Start()
		{
			printf("[D] Socket[%s] -> Start()\n", this->id.c_str());
			auto retval = bind(this->_sock, reinterpret_cast<struct sockaddr *>(&this->_local), sizeof this->_local);
			if (retval == SOCKET_ERROR)
			{
				printf("[E] Failed to bind socket \"%s\" on interfce!\n", this->id.c_str());
				this->Close();
				return;
			}

			this->bound = true;
		}

		void Socket::Listen()
		{
			if (!this->bound)
				return;

			printf("[D] Socket[%s] -> Listen()\n", this->id.c_str());
			std::thread t([this]()
						  { this->ReceiveFrom(this); });
			t.detach();
		}

		Socket::Socket(const _STRING &id, const _IPADDR &address, const _USHORT &port)
		{
			this->id = id;
			this->port = htons(port);
			this->address = address;
			this->bound = false;
		}

		Socket::~Socket()
		{
		}

		void Socket::ReceiveFrom(const Socket *socket)
		{
			socklen_t hostAddrSize = 0;
			_BYTE tempBuffer[UINT16_MAX];

			while (socket->bound)
			{
				hostAddrSize = sizeof(sockaddr_in);

				auto messageLength = recvfrom(socket->_sock, tempBuffer, sizeof tempBuffer, 0,
											  (sockaddr *)&socket->_remote, &hostAddrSize);

				if (messageLength <= 0)
					break;

				tempBuffer[messageLength] = '\0';

				if (socket->SocketDataReceived)
					socket->SocketDataReceived(this->id, tempBuffer, messageLength);
			}
		}

		void Socket::HeartBeat()
		{
			if (!this->bound)
				return;
		}

		void Socket::Close()
		{
			printf("[D] Socket[%s] -> Close()\n", this->id.c_str());

			_close(this->_sock);

			this->bound = false;
		}
	}
}