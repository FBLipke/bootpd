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

#include "../../Environment/Environment.h"

namespace bootp
{
	namespace Common
	{
		class IDatabase
		{
		public:
			virtual ~IDatabase() = default;

			virtual _BOOL connect(const _STRING &path) = 0;
			virtual void close() = 0;
			static std::unique_ptr<IDatabase> create(const _STRING &type);
		};
	}
}
