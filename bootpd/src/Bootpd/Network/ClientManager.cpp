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

#include "ClientManager.h"

namespace bootp
{
	namespace Network
	{
		ClientManager::ClientManager()
		{
		}

		ClientManager::~ClientManager()
		{
		}

		_BOOL ClientManager::Init(const _INT32 &argc, const char *argv[])
		{
			this->Remove = [&](const _STRING &id)
			{
				this->clients.at(id).get()->Close();
				this->clients.erase(id);
			};

			this->Add = [&](const _STRING &id, const _IPADDR &ip, const _USHORT &port)
			{
				this->clients.emplace(id, std::make_unique<Client>(id, ip, port));

				return id;
			};

			return true;
		}

		_BOOL ClientManager::Start()
		{
			for (const auto &s : this->clients)
			{
				s.second.get()->Start();
				s.second.get()->Listen();
			}

			return true;
		}

		void ClientManager::HeartBeat()
		{
			for (const auto &s : this->clients)
				s.second.get()->HeartBeat();
		}

		void ClientManager::Close()
		{
			for (const auto &s : this->clients)
				Remove(s.first);

			this->clients.clear();
		}
	}
}
