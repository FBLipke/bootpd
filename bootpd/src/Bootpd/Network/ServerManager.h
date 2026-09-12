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
	class ServerManager : public IBootpd
	{
	public:
		ServerManager();
		~ServerManager();

#ifdef _WIN32
		bool Init_Winsock(int major, int minor);
		bool Close_Winsock();
#endif

		bool Init(const int argc, const char* argv[]);

		bool Start();

		void HeartBeat();

		void Close();
	private:
#ifdef WIN32
		WSADATA wsa;
#endif
		std::vector<std::unique_ptr<bootp::Network::IServer>> servers;
	};
}
