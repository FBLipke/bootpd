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
        if (value <= std::numeric_limits<_USHORT>::max())
            tokens.push_back(static_cast<_USHORT>(value));
    }

    return tokens;
}

_INT32 Functions::__inet_addr(const _INT32 &af, const _STRING &ip_str, void *addr)
{
    return inet_pton(AF_INET, ip_str.c_str(), addr);
}
