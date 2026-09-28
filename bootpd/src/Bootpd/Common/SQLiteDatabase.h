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

#include "IDatabase.h"
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

			_BOOL save_client(const Client &client) override;
			_BOOL update_client(const Client &client) override;
			Client *find_client_by_mac(const _STRING &mac) override;
			Client *find_client_by_ip(const _STRING &ip) override;
			std::vector<Client> get_all_clients() override;
			std::vector<Client> get_expired_leases() override;
			_BOOL delete_client(_INT32 id) override;
			_BOOL delete_expired_leases() override;

			// Client options
			_BOOL set_client_option(_INT32 client_id, _INT32 option_code, const _STRING &value) override;
			_BOOL delete_client_option(_INT32 client_id, _INT32 option_code) override;
			_BOOL delete_all_client_options(_INT32 client_id) override;
			std::vector<ClientOption> get_client_options(_INT32 client_id) override;
			std::map<_INT32, _STRING> get_client_options_map(_INT32 client_id) override;

			// Client keys
			_BOOL add_client_key(const ClientKey &key) override;
			_BOOL delete_client_key(_INT32 key_id) override;
			_BOOL delete_all_client_keys(_INT32 client_id) override;
			std::vector<ClientKey> get_client_keys(_INT32 client_id) override;
			ClientKey *get_client_key_by_fingerprint(const _STRING &fingerprint) override;

		private:
			sqlite3 *db;
			_STRING db_path;

			_BOOL execute(const _STRING &sql);
			Client *parse_client(sqlite3_stmt *stmt);
			ClientOption *parse_client_option(sqlite3_stmt *stmt);
			ClientKey *parse_client_key(sqlite3_stmt *stmt);
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
