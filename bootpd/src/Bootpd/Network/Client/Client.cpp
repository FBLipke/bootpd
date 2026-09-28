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

#include "Client.h"

namespace bootp
{
	namespace Network
	{
		Client::Client(const _STRING &id, const _IPADDR &ip, const _USHORT &port)
		{
			this->ip = ip;
			this->port = port;
			this->id = id;
		}

		Client::~Client()
		{
		}

		void Client::Init()
		{
		}

		void Client::Start()
		{
		}

		void Client::Listen()
		{
			// Add TCP-Logic here :/
		}

		void Client::HeartBeat()
		{
		}

		void Client::Close()
		{
		}
	}
}