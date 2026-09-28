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
		struct Client
		{
			_INT32 id;
			_STRING mac;
			_STRING ip;
			_STRING hostname;
			_STRING bootfile;
			_INT32 lease_start;
			_INT32 lease_end;
			_INT32 created_at;
			_INT32 updated_at;
		};

		struct ClientOption
		{
			_INT32 id;
			_INT32 client_id;
			_INT32 option_code;
			_STRING value;
		};

		struct ClientKey
		{
			_INT32 id;
			_INT32 client_id;
			_STRING key_type;	 // ssh-rsa, ecdsa-sha2-nistp256, ed25519
			_STRING key_data;	 // the actual public key
			_STRING fingerprint; // SHA256 fingerprint
			_INT32 created_at;
		};

		class IDatabase
		{
		public:
			virtual ~IDatabase() = default;

			virtual _BOOL connect(const _STRING &path) = 0;
			virtual void close() = 0;

			virtual _BOOL save_client(const Client &client) = 0;
			virtual _BOOL update_client(const Client &client) = 0;
			virtual Client *find_client_by_mac(const _STRING &mac) = 0;
			virtual Client *find_client_by_ip(const _STRING &ip) = 0;
			virtual std::vector<Client> get_all_clients() = 0;
			virtual std::vector<Client> get_expired_leases() = 0;
			virtual _BOOL delete_client(_INT32 id) = 0;
			virtual _BOOL delete_expired_leases() = 0;

			// Client options (DHCP options per client)
			virtual _BOOL set_client_option(_INT32 client_id, _INT32 option_code, const _STRING &value) = 0;
			virtual _BOOL delete_client_option(_INT32 client_id, _INT32 option_code) = 0;
			virtual _BOOL delete_all_client_options(_INT32 client_id) = 0;
			virtual std::vector<ClientOption> get_client_options(_INT32 client_id) = 0;
			virtual std::map<_INT32, _STRING> get_client_options_map(_INT32 client_id) = 0;

			// Client keys (SSH public keys, certificates, etc.)
			virtual _BOOL add_client_key(const ClientKey &key) = 0;
			virtual _BOOL delete_client_key(_INT32 key_id) = 0;
			virtual _BOOL delete_all_client_keys(_INT32 client_id) = 0;
			virtual std::vector<ClientKey> get_client_keys(_INT32 client_id) = 0;
			virtual ClientKey *get_client_key_by_fingerprint(const _STRING &fingerprint) = 0;

			// Factory method
			static std::unique_ptr<IDatabase> create(const _STRING &type);
		};
	}
}
