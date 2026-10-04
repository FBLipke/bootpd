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

// Exported wrapper for plugins - static class members can't cross DLL boundaries on Windows
__LIBEXPORT std::vector<_USHORT> SplitUSHORT_Export(const std::string &s, char delimiter);

namespace bootp
{
	class Functions
	{
	public:
		static _STRING GenerateUUID();
		static void __memcpy(void *dst, const void *src, const _SIZET &length);

		static _INT32 __inet_addr(const _INT32 &af, const _STRING &ip_str, void *addr);
		static std::string __inet_ntoa(const _IPADDR &ip, const _INT32 &family);
		static _USHORT AsUSHORT(const char *input);
		static std::string Get_Hostname();
		static std::vector<_USHORT> Split_USHORT(const std::string &s, char delimiter);
		static std::string MacAsString(char *macBuffer);
		static _INT32 RoundToInteger(double value);
		static std::vector<std::string> Split(const std::string &str, const std::string &token);
		static bool Compare(const char *p1, const char *p2, const _SIZET &length);
		static std::string Replace(std::string &str, const std::string &from, const std::string &to);
		static bool CompareIPAddress(const _IPADDR &ip1, const _IPADDR &ip2, const _SIZET &length);
		static std::string AsString(const _SIZET &input);
		static void ExtractString(const char *buf, const _SIZET &size, char *out);
		static _SIZET Strip(const char *buffer, const _SIZET buflen);
	};
}
