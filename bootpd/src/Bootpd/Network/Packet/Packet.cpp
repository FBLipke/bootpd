#include "Packet.h"

namespace bootp::Network
{
    Packet::~Packet()
    {
        this->buffer.clear();
    }

    Packet::Packet(const char *data, const _SIZET &len)
    {
        this->buffer.assign(data, data + len);
    }
}
