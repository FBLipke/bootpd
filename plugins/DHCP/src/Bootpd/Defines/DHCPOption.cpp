#include "DHCPOption.h"

DHCPOption::DHCPOption(/* args */)
{
}

DHCPOption::~DHCPOption()
{
}

const uint8_t &DHCPOption::Get_Option() const
{
    return this->opt;
}

const uint8_t &DHCPOption::Get_Length() const
{
    return this->len;
}

const char *DHCPOption::Get_Data() const
{
    return this->data;
}

const uint16_t DHCPOption::As_Uint16()
{
    uint16_t val = 0;

    std::copy(this->data, this->data + sizeof(uint16_t), &val);

    return 0;
}

const uint32_t DHCPOption::As_Uint32()
{
    uint32_t val = 0;

    std::copy(this->data, this->data + sizeof(uint32_t), &val);

    return 0;
}
