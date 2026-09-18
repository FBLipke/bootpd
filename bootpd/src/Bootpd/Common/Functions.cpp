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

    result = (_BYTE *)str;
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
        static const _BYTE *hex = "0123456789abcdef";
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

void Functions::__memcpy(void *dst, const void *src, _SIZET length)
{
    std::copy(static_cast<const _BYTE *>(src),
              static_cast<const _BYTE *>(src) + length,
              static_cast<_BYTE *>(dst));
}

_INT32 Functions::__inet_addr(const _INT32 &af, const _STRING &ip_str, void *addr)
{
    return inet_pton(AF_INET, ip_str.c_str(), &addr);
}
