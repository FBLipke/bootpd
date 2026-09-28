#include "../../Common/Environment.h"
#pragma once

namespace bootp::Network
{
    class IPacket
    {
    public:
        virtual ~IPacket() = default;
    };
}