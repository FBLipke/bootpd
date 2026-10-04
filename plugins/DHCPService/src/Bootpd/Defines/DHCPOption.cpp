#include "DHCPOption.h"
namespace bootp::Plugins::DHCP
{
    DHCPOption::DHCPOption(const _BYTE &opt)
    {
        this->opt = opt;
        this->len = 0;

        this->data = new char[0];
    }

    DHCPOption::DHCPOption(const _BYTE &opt, const _BYTE &data)
    {
        this->opt = opt;
        this->len = 1;
        this->data = new char[this->Get_Length()];
        memcpy(this->data, &data, this->len);
    }

    DHCPOption::DHCPOption(const _BYTE &opt, const _USHORT &data)
    {
        this->opt = opt;
        this->len = static_cast<_BYTE>(sizeof(_USHORT));

        memcpy(this->data, &data, this->len);
    }

    DHCPOption::DHCPOption(const _BYTE &opt, const _UINT &data)
    {
        this->opt = opt;
        this->len = static_cast<_BYTE>(sizeof(_UINT));

        memcpy(this->data, &data, this->len);
    }

    DHCPOption::DHCPOption(const _BYTE &opt, const _STRING &data)
    {
        this->opt = opt;
        this->len = static_cast<_BYTE>(data.size());

        memcpy(this->data, data.c_str(), this->len);
    }

    DHCPOption::DHCPOption::~DHCPOption()
    {
        if (this->data != nullptr)
        {
            delete this->data;
            this->data = nullptr;
        }
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

        return val;
    }

    const uint32_t DHCPOption::As_Uint32()
    {
        uint32_t val = 0;

        std::copy(this->data, this->data + sizeof(uint32_t), &val);

        return val;
    }
}
