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
namespace bootp
{

	namespace Network
	{
		class ClientManager : public IBootpd
		{
		public:
			ClientManager();
			~ClientManager();

			_BOOL Init(const _INT32 &argc, const char *argv[]);

			_BOOL Start();

			void HeartBeat();

			void Close();
			std::function<_STRING(const _STRING &id, const _IPADDR &ip, const _USHORT &port)> Add;
			std::function<void(const _STRING &id)> Remove;
			std::function<void(const _STRING &server_id, const _STRING &socket_id, const std::shared_ptr<IPacket> &request, const _IPADDR &ip, const _USHORT &port, const _STRING &client)> Handle_Manager_Request;
			std::function<void(const _STRING &server_id, const _STRING &socket_id, const std::shared_ptr<IPacket> &request, const _IPADDR &ip, const _USHORT &port, const _STRING &client)> Handle_Manager_Response;

			void Add_Server(const std::vector<_USHORT> &ports) override;

		private:
#ifdef WIN32
			WSADATA wsa;
#endif
			std::map<const _STRING, std::unique_ptr<bootp::Network::IClient>> clients;
		};
	}
}
