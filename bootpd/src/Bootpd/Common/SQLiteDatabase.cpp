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

		Client *SQLiteDatabase::parse_client(sqlite3_stmt *stmt)
		{
			if (!stmt)
				return nullptr;

			Client *client = new Client();
			client->id = sqlite3_column_int(stmt, 0);
			client->mac = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
			client->ip = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
			client->hostname = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
			client->bootfile = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
			client->lease_start = sqlite3_column_int(stmt, 5);
			client->lease_end = sqlite3_column_int(stmt, 6);
			client->created_at = sqlite3_column_int(stmt, 7);
			client->updated_at = sqlite3_column_int(stmt, 8);
			return client;
		}

		ClientOption *SQLiteDatabase::parse_client_option(sqlite3_stmt *stmt)
		{
			if (!stmt)
				return nullptr;

			ClientOption *opt = new ClientOption();
			opt->id = sqlite3_column_int(stmt, 0);
			opt->client_id = sqlite3_column_int(stmt, 1);
			opt->option_code = sqlite3_column_int(stmt, 2);
			opt->value = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
			return opt;
		}

		ClientKey *SQLiteDatabase::parse_client_key(sqlite3_stmt *stmt)
		{
			if (!stmt)
				return nullptr;

			ClientKey *key = new ClientKey();
			key->id = sqlite3_column_int(stmt, 0);
			key->client_id = sqlite3_column_int(stmt, 1);
			key->key_type = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
			key->key_data = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
			key->fingerprint = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
			key->created_at = sqlite3_column_int(stmt, 5);
			return key;
		}

		_BOOL SQLiteDatabase::save_client(const Client &client)
		{
			if (!db)
				return false;

			_STRING sql = "INSERT OR REPLACE INTO clients (mac, ip, hostname, bootfile, lease_start, lease_end, created_at, updated_at) VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
			sqlite3_stmt *stmt;

			if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
			{
				std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
				return false;
			}

			sqlite3_bind_text(stmt, 1, client.mac.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 2, client.ip.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 3, client.hostname.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 4, client.bootfile.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(stmt, 5, client.lease_start);
			sqlite3_bind_int(stmt, 6, client.lease_end);
			sqlite3_bind_int(stmt, 7, client.created_at);
			sqlite3_bind_int(stmt, 8, client.updated_at);

			_BOOL success = (sqlite3_step(stmt) == SQLITE_DONE);
			if (!success)
			{
				std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
			}

			sqlite3_finalize(stmt);
			return success;
		}

		_BOOL SQLiteDatabase::update_client(const Client &client)
		{
			if (!db)
				return false;

			_STRING sql = "UPDATE clients SET ip = ?, hostname = ?, bootfile = ?, lease_start = ?, lease_end = ?, updated_at = ? WHERE id = ?";
			sqlite3_stmt *stmt;

			if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
			{
				std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
				return false;
			}

			sqlite3_bind_text(stmt, 1, client.ip.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 2, client.hostname.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 3, client.bootfile.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(stmt, 4, client.lease_start);
			sqlite3_bind_int(stmt, 5, client.lease_end);
			sqlite3_bind_int(stmt, 6, client.updated_at);
			sqlite3_bind_int(stmt, 7, client.id);

			_BOOL success = (sqlite3_step(stmt) == SQLITE_DONE);
			if (!success)
			{
				std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
			}

			sqlite3_finalize(stmt);
			return success;
		}

		Client *SQLiteDatabase::find_client_by_mac(const _STRING &mac)
		{
			if (!db)
				return nullptr;

			_STRING sql = "SELECT id, mac, ip, hostname, bootfile, lease_start, lease_end, created_at, updated_at FROM clients WHERE mac = ?";
			sqlite3_stmt *stmt;

			if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
				return nullptr;

			sqlite3_bind_text(stmt, 1, mac.c_str(), -1, SQLITE_TRANSIENT);

			Client *client = nullptr;
			if (sqlite3_step(stmt) == SQLITE_ROW)
				client = parse_client(stmt);

			sqlite3_finalize(stmt);
			return client;
		}

		Client *SQLiteDatabase::find_client_by_ip(const _STRING &ip)
		{
			if (!db)
				return nullptr;

			_STRING sql = "SELECT id, mac, ip, hostname, bootfile, lease_start, lease_end, created_at, updated_at FROM clients WHERE ip = ?";
			sqlite3_stmt *stmt;

			if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
				return nullptr;

			sqlite3_bind_text(stmt, 1, ip.c_str(), -1, SQLITE_TRANSIENT);

			Client *client = nullptr;
			if (sqlite3_step(stmt) == SQLITE_ROW)
				client = parse_client(stmt);

			sqlite3_finalize(stmt);
			return client;
		}

		std::vector<Client> SQLiteDatabase::get_all_clients()
		{
			std::vector<Client> clients;

			if (!db)
				return clients;

			_STRING sql = "SELECT id, mac, ip, hostname, bootfile, lease_start, lease_end, created_at, updated_at FROM clients";
			sqlite3_stmt *stmt;

			if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
				return clients;

			while (sqlite3_step(stmt) == SQLITE_ROW)
			{
				if (Client *client = parse_client(stmt))
				{
					clients.push_back(*client);
					delete client;
				}
			}

			sqlite3_finalize(stmt);
			return clients;
		}

		std::vector<Client> SQLiteDatabase::get_expired_leases()
		{
			std::vector<Client> clients;

			if (!db)
				return clients;

			_INT32 now = static_cast<_INT32>(time(nullptr));
			_STRING sql = "SELECT id, mac, ip, hostname, bootfile, lease_start, lease_end, created_at, updated_at FROM clients WHERE lease_end < ?";
			sqlite3_stmt *stmt;

			if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
				return clients;

			sqlite3_bind_int(stmt, 1, now);

			while (sqlite3_step(stmt) == SQLITE_ROW)
			{
				if (Client *client = parse_client(stmt))
				{
					clients.push_back(*client);
					delete client;
				}
			}

			sqlite3_finalize(stmt);
			return clients;
		}

		_BOOL SQLiteDatabase::delete_client(_INT32 id)
		{
			if (!db)
				return false;

			_STRING sql = "DELETE FROM clients WHERE id = ?";
			sqlite3_stmt *stmt;

			if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
				return false;

			sqlite3_bind_int(stmt, 1, id);
			_BOOL success = (sqlite3_step(stmt) == SQLITE_DONE);
			sqlite3_finalize(stmt);
			return success;
		}

		_BOOL SQLiteDatabase::delete_expired_leases()
		{
			if (!db)
				return false;

			_INT32 now = static_cast<_INT32>(time(nullptr));
			_STRING sql = "DELETE FROM clients WHERE lease_end < ?";
			sqlite3_stmt *stmt;

			if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
				return false;

			sqlite3_bind_int(stmt, 1, now);
			_BOOL success = (sqlite3_step(stmt) == SQLITE_DONE);
			sqlite3_finalize(stmt);
			return success;
		}

		// Client Options

		_BOOL SQLiteDatabase::set_client_option(_INT32 client_id, _INT32 option_code, const _STRING &value)
		{
			if (!db)
				return false;

			_STRING sql = "INSERT OR REPLACE INTO client_options (client_id, option_code, value) VALUES (?, ?, ?)";
			sqlite3_stmt *stmt;

			if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
				return false;

			sqlite3_bind_int(stmt, 1, client_id);
			sqlite3_bind_int(stmt, 2, option_code);
			sqlite3_bind_text(stmt, 3, value.c_str(), -1, SQLITE_TRANSIENT);

			_BOOL success = (sqlite3_step(stmt) == SQLITE_DONE);
			sqlite3_finalize(stmt);
			return success;
		}

		_BOOL SQLiteDatabase::delete_client_option(_INT32 client_id, _INT32 option_code)
		{
			if (!db)
				return false;

			_STRING sql = "DELETE FROM client_options WHERE client_id = ? AND option_code = ?";
			sqlite3_stmt *stmt;

			if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
				return false;

			sqlite3_bind_int(stmt, 1, client_id);
			sqlite3_bind_int(stmt, 2, option_code);

			_BOOL success = (sqlite3_step(stmt) == SQLITE_DONE);
			sqlite3_finalize(stmt);
			return success;
		}

		_BOOL SQLiteDatabase::delete_all_client_options(_INT32 client_id)
		{
			if (!db)
				return false;

			_STRING sql = "DELETE FROM client_options WHERE client_id = ?";
			sqlite3_stmt *stmt;

			if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
				return false;

			sqlite3_bind_int(stmt, 1, client_id);
			_BOOL success = (sqlite3_step(stmt) == SQLITE_DONE);
			sqlite3_finalize(stmt);
			return success;
		}

		std::vector<ClientOption> SQLiteDatabase::get_client_options(_INT32 client_id)
		{
			std::vector<ClientOption> options;

			if (!db)
				return options;

			_STRING sql = "SELECT id, client_id, option_code, value FROM client_options WHERE client_id = ?";
			sqlite3_stmt *stmt;

			if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
				return options;

			sqlite3_bind_int(stmt, 1, client_id);

			while (sqlite3_step(stmt) == SQLITE_ROW)
			{
				if (ClientOption *opt = parse_client_option(stmt))
				{
					options.push_back(*opt);
					delete opt;
				}
			}

			sqlite3_finalize(stmt);
			return options;
		}

		std::map<_INT32, _STRING> SQLiteDatabase::get_client_options_map(_INT32 client_id)
		{
			std::map<_INT32, _STRING> options;

			if (!db)
				return options;

			_STRING sql = "SELECT option_code, value FROM client_options WHERE client_id = ?";
			sqlite3_stmt *stmt;

			if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
				return options;

			sqlite3_bind_int(stmt, 1, client_id);

			while (sqlite3_step(stmt) == SQLITE_ROW)
			{
				_INT32 code = sqlite3_column_int(stmt, 0);
				_STRING value = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
				options[code] = value;
			}

			sqlite3_finalize(stmt);
			return options;
		}

		// Client Keys

		_BOOL SQLiteDatabase::add_client_key(const ClientKey &key)
		{
			if (!db)
				return false;

			_STRING sql = "INSERT INTO client_keys (client_id, key_type, key_data, fingerprint, created_at) VALUES (?, ?, ?, ?, ?)";
			sqlite3_stmt *stmt;

			if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
				return false;

			sqlite3_bind_int(stmt, 1, key.client_id);
			sqlite3_bind_text(stmt, 2, key.key_type.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 3, key.key_data.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 4, key.fingerprint.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(stmt, 5, key.created_at);

			_BOOL success = (sqlite3_step(stmt) == SQLITE_DONE);
			sqlite3_finalize(stmt);
			return success;
		}

		_BOOL SQLiteDatabase::delete_client_key(_INT32 key_id)
		{
			if (!db)
				return false;

			_STRING sql = "DELETE FROM client_keys WHERE id = ?";
			sqlite3_stmt *stmt;

			if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
				return false;

			sqlite3_bind_int(stmt, 1, key_id);
			_BOOL success = (sqlite3_step(stmt) == SQLITE_DONE);
			sqlite3_finalize(stmt);
			return success;
		}

		_BOOL SQLiteDatabase::delete_all_client_keys(_INT32 client_id)
		{
			if (!db)
				return false;

			_STRING sql = "DELETE FROM client_keys WHERE client_id = ?";
			sqlite3_stmt *stmt;

			if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
				return false;

			sqlite3_bind_int(stmt, 1, client_id);
			_BOOL success = (sqlite3_step(stmt) == SQLITE_DONE);
			sqlite3_finalize(stmt);
			return success;
		}

		std::vector<ClientKey> SQLiteDatabase::get_client_keys(_INT32 client_id)
		{
			std::vector<ClientKey> keys;

			if (!db)
				return keys;

			_STRING sql = "SELECT id, client_id, key_type, key_data, fingerprint, created_at FROM client_keys WHERE client_id = ?";
			sqlite3_stmt *stmt;

			if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
				return keys;

			sqlite3_bind_int(stmt, 1, client_id);

			while (sqlite3_step(stmt) == SQLITE_ROW)
			{
				if (ClientKey *key = parse_client_key(stmt))
				{
					keys.push_back(*key);
					delete key;
				}
			}

			sqlite3_finalize(stmt);
			return keys;
		}

		ClientKey *SQLiteDatabase::get_client_key_by_fingerprint(const _STRING &fingerprint)
		{
			if (!db)
				return nullptr;

			_STRING sql = "SELECT id, client_id, key_type, key_data, fingerprint, created_at FROM client_keys WHERE fingerprint = ?";
			sqlite3_stmt *stmt;

			if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
				return nullptr;

			sqlite3_bind_text(stmt, 1, fingerprint.c_str(), -1, SQLITE_TRANSIENT);

			ClientKey *key = nullptr;
			if (sqlite3_step(stmt) == SQLITE_ROW)
				key = parse_client_key(stmt);

			sqlite3_finalize(stmt);
			return key;
		}
	} // namespace Common
} // namespace bootp
