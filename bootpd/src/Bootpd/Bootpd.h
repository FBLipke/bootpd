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
#include "Common/Environment.h"
#include "Common/Functions.h"
#include "Interface/IBootpd.h"
#include "Network/Interface/ISocket.h"
#include "Network/Socket.h"
#include "Network/Interface/IServer.h"
#include "Network/Server.h"
#include "Network/ServerManager.h"
#include "Services/ServiceManager.h"

namespace bootp
{
	class bootpd : public IBootpd
	{
	public:
		bootpd();
		~bootpd();

		void Add_Subsys(const _STRING &str, std::unique_ptr<IBootpd> subsys);

		IBootpd *Get_SubSystem(const _STRING &id);

		bool Init(const _INT32 &argc, const _BYTE *argv[]) override;

		bool Start() override;

		void HeartBeat() override;

		void Close() override;
	};
}
