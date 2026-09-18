#include "../Bootpd.h"
#pragma once

class DHCPOption
{

public:
    DHCPOption(/* args */);
    ~DHCPOption();

    const uint8_t &Get_Option() const;

    const uint8_t &Get_Length() const;

    const char *Get_Data() const;

    const uint16_t As_Uint16();

    const uint32_t As_Uint32();

private:
    uint8_t opt;
    uint8_t len;
    char *data;
};
