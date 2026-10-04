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

			this->proto = 0;  // UDP: 0 = auto

			printf("[D] Socket[%s] -> Init(%u)\n",
				   this->id.c_str(), htons(this->port));

			this->socketType = SOCK_DGRAM;

			this->_sock = socket(af, this->socketType, this->proto);
			printf("[D] Socket: socket() returned %d, WSA error=%d\n", this->_sock, WSAGetLastError());
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

			while (socket->bound)
			{
				socklen_t hostAddrSize = 0;
				char tempBuffer[UINT16_MAX];
				hostAddrSize = sizeof(sockaddr_in);
				sockaddr_in _remote;
				memset(&_remote, 0x00, sizeof(_remote));

				auto messageLength = recvfrom(socket->_sock, tempBuffer, sizeof(tempBuffer), 0,
											  reinterpret_cast<sockaddr *>(&_remote), &hostAddrSize);

				if (messageLength <= 0)
					break;

				tempBuffer[messageLength] = '\0';

				const std::shared_ptr<IPacket> request = std::make_shared<Packet>(tempBuffer, messageLength);

				_IPADDR _ip = ntohl(_remote.sin_addr.s_addr);
				_USHORT _port = ntohs(_remote.sin_port);
				_STRING _client = Functions::GenerateUUID();
				if (socket->SocketDataReceived)
					socket->SocketDataReceived(this->id, request, _ip, _port, _client);
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