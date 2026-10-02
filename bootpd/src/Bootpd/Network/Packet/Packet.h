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
            this->curPosition = this->position;

            T value = 0;

            for (_SIZET i = 0; i < sizeof(T); ++i)
                if (little_Endian)
                    value |= static_cast<T>(this->buffer.at(offset + i)) << (i * 8);
                else
                    value |= static_cast<T>(this->buffer.at(offset + i)) << ((sizeof(T) - 1 - i) * 8);

            this->position = this->curPosition;

            return value;
        }

        template <typename T>
        void Write(const _SIZET &offset, const T &value, const _BOOL &little_Endian = true)
        {
            this->curPosition = this->position;

            for (_SIZET i = 0; i < sizeof(T); ++i)
                if (little_Endian)
                    this->buffer.at(offset + i) = static_cast<char>((value >> (i * 8)) & 0xFF);
                else
                    this->buffer.at(offset + (sizeof(T) - 1 - i)) = static_cast<char>((value >> (i * 8)) & 0xFF);

            this->position = this->curPosition;
        }

        const _SIZET Get_Length() const
        {
            return this->buffer.size();
        }

        const _SIZET &Get_Position() const
        {
            return this->position;
        }

        void Set_Position(const _SIZET &pos)
        {
            this->position = pos;
        }

    protected:
        std::vector<char> buffer;
        _SIZET position = 0;
        _SIZET curPosition = 0;
    };
}
