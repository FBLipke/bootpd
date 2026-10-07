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

#include "Interface/IDatabase.h"
#include "sqlite3/sqlite3.h"

namespace bootp
{
	namespace Common
	{
		class SQLiteDatabase : public IDatabase
		{
		public:
			SQLiteDatabase();
			~SQLiteDatabase() override;

			_BOOL connect(const _STRING &path) override;
			void close() override;

		private:
			sqlite3 *db;
			_STRING db_path;

			_BOOL execute(const _STRING &sql);
		};

		// Factory implementation
		inline std::unique_ptr<IDatabase> IDatabase::create(const _STRING &type)
		{
			if (type == "sqlite" || type == "sqlite3")
			{
				return std::make_unique<SQLiteDatabase>();
			}
			// Add more database types here:
			// if (type == "mysql") return std::make_unique<MySQLDatabase>();
			// if (type == "postgres") return std::make_unique<PostgresDatabase>();
			return nullptr;
		}
	}
}
