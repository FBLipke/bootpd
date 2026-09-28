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

#include "Environment.h"

namespace bootp
{
	namespace Common
	{
		class IDatabase;

		class DatabaseConfig
		{
		public:
			// Database type
			_STRING type;

			// SQLite specific
			_STRING path;

			// MySQL/PostgreSQL specific
			_STRING host;
			_INT32 port;
			_STRING database;
			_STRING username;
			_STRING password;

			// Extra options (key-value pairs)
			std::map<_STRING, _STRING> options;

			DatabaseConfig() : port(0) {}

			// Load from key-value map (e.g., from config file)
			static DatabaseConfig fromMap(const std::map<_STRING, _STRING> &params);

			// Factory method - creates database instance from config
			static std::unique_ptr<IDatabase> createDatabase(const DatabaseConfig &config);
		};
	}
}
