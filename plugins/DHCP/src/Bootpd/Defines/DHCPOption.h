#pragma once
#include "../Bootpd.h"
class DHCPOption
{

public:
    DHCPOption(const _BYTE &opt);

    DHCPOption(const _BYTE &opt, const _BYTE &data);

    DHCPOption(const _BYTE &opt, const _USHORT &data);

    DHCPOption(const _BYTE &opt, const _UINT &data);

    DHCPOption(const _BYTE &opt, const _STRING &data);

    ~DHCPOption();

    const uint8_t &Get_Option() const;

    const uint8_t &Get_Length() const;

    const char *Get_Data() const;

    const uint16_t As_Uint16();

    const uint32_t As_Uint32();

private:
    uint8_t opt = 0;
    uint8_t len = 0;
    char *data;
};
