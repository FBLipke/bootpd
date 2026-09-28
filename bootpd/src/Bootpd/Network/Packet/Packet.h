#include "../../Common/Environment.h"
#include "../Interface/IPacket.h"

#pragma once

namespace bootp::Network
{
    class Packet : public IPacket
    {
    public:
        Packet() = default;

        explicit Packet(const char *data, const _SIZET &len);
        ~Packet() override;

        template <typename T>
        T Read(const _SIZET &offset, const _BOOL &little_Endian = true)
        {
            T value = 0;

            for (_SIZET i = 0; i < sizeof(T); ++i)
                if (little_Endian)
                    value |= static_cast<T>(this->buffer.at(offset + i)) << (i * 8);
                else
                    value |= static_cast<T>(this->buffer.at(offset + i)) << ((sizeof(T) - 1 - i) * 8);

            return value;
        }

    protected:
        std::vector<char> buffer;
    };
}
