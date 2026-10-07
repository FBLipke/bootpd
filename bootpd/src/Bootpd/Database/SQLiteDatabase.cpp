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

#include "SQLiteDatabase.h"

namespace bootp
{
	namespace Common
	{
		SQLiteDatabase::SQLiteDatabase() : db(nullptr)
		{
		}

		SQLiteDatabase::~SQLiteDatabase()
		{
			close();
		}

		_BOOL SQLiteDatabase::connect(const _STRING &path)
		{
			db_path = path;
			_INT32 result = sqlite3_open(path.c_str(), &db);
			if (result != SQLITE_OK)
			{
				std::cerr << "Failed to open database: " << sqlite3_errmsg(db) << std::endl;
				db = nullptr;
				return false;
			}

			// Enable foreign keys
			sqlite3_exec(db, "PRAGMA foreign_keys = ON", nullptr, nullptr, nullptr);

			// Create tables if not exist
			const char *create_table_sql = R"(
				CREATE TABLE IF NOT EXISTS clients (
					id INTEGER PRIMARY KEY AUTOINCREMENT,
					mac TEXT NOT NULL UNIQUE,
					ip TEXT NOT NULL,
					hostname TEXT,
					bootfile TEXT,
					lease_start INTEGER NOT NULL,
					lease_end INTEGER NOT NULL,
					created_at INTEGER NOT NULL,
					updated_at INTEGER NOT NULL
				);
				CREATE INDEX IF NOT EXISTS idx_clients_mac ON clients(mac);
				CREATE INDEX IF NOT EXISTS idx_clients_ip ON clients(ip);
				CREATE INDEX IF NOT EXISTS idx_clients_lease_end ON clients(lease_end);

				CREATE TABLE IF NOT EXISTS client_options (
					id INTEGER PRIMARY KEY AUTOINCREMENT,
					client_id INTEGER NOT NULL REFERENCES clients(id) ON DELETE CASCADE,
					option_code INTEGER NOT NULL,
					value TEXT NOT NULL,
					UNIQUE(client_id, option_code)
				);
				CREATE INDEX IF NOT EXISTS idx_client_options_client_id ON client_options(client_id);

				CREATE TABLE IF NOT EXISTS client_keys (
					id INTEGER PRIMARY KEY AUTOINCREMENT,
					client_id INTEGER NOT NULL REFERENCES clients(id) ON DELETE CASCADE,
					key_type TEXT NOT NULL,
					key_data TEXT NOT NULL,
					fingerprint TEXT NOT NULL UNIQUE,
					created_at INTEGER NOT NULL
				);
				CREATE INDEX IF NOT EXISTS idx_client_keys_client_id ON client_keys(client_id);
				CREATE INDEX IF NOT EXISTS idx_client_keys_fingerprint ON client_keys(fingerprint);
			)";

			return execute(create_table_sql);
		}

		void SQLiteDatabase::close()
		{
			if (db)
			{
				sqlite3_close(db);
				db = nullptr;
			}
		}

		_BOOL SQLiteDatabase::execute(const _STRING &sql)
		{
			if (!db)
			{
				std::cerr << "Database not connected" << std::endl;
				return false;
			}

			char *err_msg = nullptr;
			_INT32 result = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &err_msg);
			if (result != SQLITE_OK)
			{
				std::cerr << "SQL error: " << err_msg << std::endl;
				sqlite3_free(err_msg);
				return false;
			}
			return true;
		}
	} // namespace Common
} // namespace bootp
