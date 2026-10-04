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

#include "Functions.h"
namespace bootp
{
	// Exported wrapper for plugins - static class members can't cross DLL boundaries on Windows
	std::vector<_USHORT> SplitUSHORT_Export(const std::string &s, char delimiter)
	{
		return Functions::Split_USHORT(s, delimiter);
	}

	_STRING Functions::GenerateUUID()
	{
		_STRING result = "";
#ifdef _WIN32
		UUID uuid;
		UuidCreate(&uuid);

		RPC_CSTR str = nullptr;
		UuidToStringA(&uuid, &str);

		result = (char *)str;
		RpcStringFreeA(&str);
#else
		std::ifstream f("/proc/sys/kernel/random/uuid");

		if (f.is_open())
		{
			std::getline(f, result);
		}
		else
		{
			// Fallback: manual UUID v4 generation
			static const char *hex = "0123456789abcdef";
			result = "00000000-0000-0000-0000-000000000000";
			for (_SIZET i = 0; i < 36; i++)
			{
				if (result[i] == '0')
				{
					_INT32 rnd = rand() % 16;
					if (i == 14)
						rnd = 4; // Version 4
					if (i == 19)
						rnd = (rand() % 4) + 8; // Variant
					result[i] = hex[rnd];
				}
			}
		}
#endif

		return result;
	}

	void Functions::__memcpy(void *dst, const void *src, const _SIZET &length)
	{
		std::copy(static_cast<const char *>(src),
				  static_cast<const char *>(src) + length,
				  static_cast<char *>(dst));
	}

	std::vector<_USHORT> Functions::Split_USHORT(const std::string &s, char delimiter)
	{
		std::vector<_USHORT> tokens;
		std::string token;
		std::istringstream iss(s);

		while (std::getline(iss, token, delimiter))
		{
			auto value = std::stoi(token);
			tokens.push_back(static_cast<_USHORT>(value));
		}

		return tokens;
	}

	_INT32 Functions::__inet_addr(const _INT32 &af, const _STRING &ip_str, void *addr)
	{
		return inet_pton(AF_INET, ip_str.c_str(), addr);
	}
	std::string Functions::__inet_ntoa(const _IPADDR &ip, const _INT32 &family)
	{
		in_addr addr;
		_ClearBuffer(&addr, sizeof addr);
		addr.s_addr = ip;

		char _addr[128];
		_ClearBuffer(_addr, sizeof _addr);

		inet_ntop(family, &addr, _addr, sizeof _addr);

		return std::string(_addr);
	}

	_USHORT Functions::AsUSHORT(const char *input)
	{
		return static_cast<_USHORT>(strtoul(input, nullptr, 0));
	}

	std::string Functions::Get_Hostname()
	{
		char hname[64];
		_ClearBuffer(hname, sizeof hname);
		gethostname(hname, sizeof hname);

		return std::string(hname);
	}

	std::string Functions::MacAsString(char *macBuffer)
	{
		char out[32];
		_ClearBuffer(out, sizeof out);
		std::string mac = std::string("");

		sprintf(out, "%02X:%02X:%02X:%02X:%02X:%02X",
				static_cast<_BYTE>(macBuffer[0]),
				static_cast<_BYTE>(macBuffer[1]),
				static_cast<_BYTE>(macBuffer[2]),
				static_cast<_BYTE>(macBuffer[3]),
				static_cast<_BYTE>(macBuffer[4]),
				static_cast<_BYTE>(macBuffer[5]));

		mac = std::string(out);

		return mac;
	}

	_INT32 Functions::RoundToInteger(double value)
	{
		return static_cast<_INT32>(round(value + 0.5));
	}

	std::vector<std::string> Functions::Split(const std::string &str, const std::string &token)
	{
		std::vector<std::string> output;
		std::string::size_type prev_pos = 0, pos = 0;

		while ((pos = str.find(token, pos)) != std::string::npos)
		{
			std::string substring(str.substr(prev_pos, pos - prev_pos));
			output.push_back(substring);
			prev_pos = ++pos;
		}

		output.push_back(str.substr(prev_pos, pos - prev_pos)); // Last word

		return output;
	}

	bool Functions::Compare(const char *p1, const char *p2, const _SIZET &length)
	{
		return memcmp(p1, p2, length) == 0;
	}

	std::string Functions::Replace(std::string &str, const std::string &from, const std::string &to)
	{
		_SIZET start_pos = str.find(from);

		while (str.find(from) != std::string::npos)
		{
			start_pos = str.find(from);

			if (start_pos != std::string::npos)
				str = str.replace(start_pos, from.length(), to);
		}

		return str;
	}

	bool Functions::CompareIPAddress(const _IPADDR &ip1, const _IPADDR &ip2, const _SIZET &length)
	{
		return memcmp(&ip1, &ip2, length) == 0;
	}

	std::string Functions::AsString(const _SIZET &input)
	{
		std::stringstream ss;
		ss << input;

		return ss.str();
	}

	void Functions::ExtractString(const char *buf, const _SIZET &size, char *out)
	{
		_ClearBuffer(out, size);
		strncpy(out, buf, size - 1);
	}

	_SIZET Functions::Strip(const char *buffer, const _SIZET buflen)
	{
		for (_SIZET i = buflen; buflen > 0; i--)
			if (static_cast<_BYTE>(buffer[i]) == static_cast<_BYTE>(0xff))
				return i + 1;

		return buflen;
	}
}
