#include "Packet.h"

namespace bootp::Network
{
    Packet::~Packet()
    {
        this->buffer.clear();
    }

    Packet::Packet(const char *data, const _SIZET &len)
    {
        if (IsNull(data))
            return;

        this->buffer.assign(data, data + len);
    }
}
