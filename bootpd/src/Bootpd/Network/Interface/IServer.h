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
#include "../../Bootpd.h"

namespace bootp
{
	namespace Network
	{
		class IServer
		{
		public:
			virtual void Init() = 0;
			virtual void Start() = 0;
			virtual void Listen() = 0;
			virtual void HeartBeat() = 0;
			virtual void Close() = 0;
			std::function<void(const _STRING &server_id, const _STRING &socket_id,
							   const _BYTE *buffer, const _SIZET &length)>
				ServerDataReceived;

		private:
			_STRING id;
		};
	}
}
