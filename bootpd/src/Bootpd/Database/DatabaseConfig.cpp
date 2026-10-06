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

#include "DatabaseConfig.h"
#include "Interface/IDatabase.h"
#include "SQLiteDatabase.h"

namespace bootp
{
	namespace Common
	{
		DatabaseConfig DatabaseConfig::fromMap(const std::map<_STRING, _STRING> &params)
		{
			DatabaseConfig config;

			// Common
			if (params.count("type"))
				config.type = params.at("type");

			// SQLite
			if (params.count("path"))
				config.path = params.at("path");

			// MySQL/PostgreSQL
			if (params.count("host"))
				config.host = params.at("host");
			if (params.count("port"))
				config.port = std::stoi(params.at("port"));
			if (params.count("database"))
				config.database = params.at("database");
			if (params.count("username"))
				config.username = params.at("username");
			if (params.count("password"))
				config.password = params.at("password");

			// Extra options
			for (const auto &[key, value] : params)
			{
				if (key != "type" && key != "path" && key != "host" &&
					key != "port" && key != "database" && key != "username" && key != "password")
				{
					config.options[key] = value;
				}
			}

			return config;
		}

		std::unique_ptr<IDatabase> DatabaseConfig::createDatabase(const DatabaseConfig &config)
		{
			// Currently only SQLite is implemented
			if (config.type == "sqlite" || config.type == "sqlite3")
			{
				return std::make_unique<SQLiteDatabase>();
			}

			// Future: MySQL, PostgreSQL, etc.
			// if (config.type == "mysql")
			//     return std::make_unique<MySQLDatabase>(config);
			// if (config.type == "postgres")
			//     return std::make_unique<PostgresDatabase>(config);

			std::cerr << "Unknown database type: " << config.type << std::endl;
			return nullptr;
		}
	}
}
