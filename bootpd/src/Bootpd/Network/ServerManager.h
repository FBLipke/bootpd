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
		class ServerManager : public IBootpd
		{
		public:
			ServerManager();
			~ServerManager();

#ifdef _WIN32
			_BOOL Init_Winsock(_INT32 major, _INT32 minor);
			_BOOL Close_Winsock();
#endif

			_BOOL Init(const _INT32 &argc, const _BYTE *argv[]);

			_BOOL Start();

			void HeartBeat();

			void Close();

			std::function<void(const _STRING &server_id, const _STRING &socket_id, const _BYTE *, const _SIZET &)> HandleManager_Request;
			std::function<void(const _STRING &server_id, const _STRING &socket_id, const _BYTE *, const _SIZET &)> HandleManager_Response;

		private:
#ifdef WIN32
			WSADATA wsa;
#endif
			std::map<const _STRING, std::unique_ptr<bootp::Network::IServer>> servers;
		};
	}
}