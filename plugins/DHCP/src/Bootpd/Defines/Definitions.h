#pragma once
#include "../Bootpd.h"

namespace bootpd::Services::DHCP
{
    enum class Architecture : _USHORT
    {
        Intel_x86PC = 0x0000,
        NEC_PC98 = 0x0001,
        EFI_Itanium = 0x0002,
        Arc_x86 = 0x0004,
        Intel_LeanClient = 0x0005,
        EFI_IA32 = 0x0006,
        EFI_BC = 0x0007,
        EFI_Xscale = 0x0008,
        EBC = 0x0009,
        ARM_32_UEFI = 0x000a,
        ARM_64_UEFI = 0x000b
    };

    enum class BootpOPCode : _BYTE
    {
        BootRequest = 0x00,
        BootReply = 0x01
    };

    enum class DHCPMessageType : _BYTE
    {
        Discover = 1,
        Offer = 2,
        Request = 3,
        Decline = 4,
        Ack = 5,
        Nak = 6,
        Release = 7,
        Inform = 8,
        LeaseQuery = 9,
        LeaseUnassigned = 10,
        LeaseUnknown = 11,
        LeaseActive = 12,
        BulkLeaseQuery = 13,
        LeaseQueryDone = 14,
        LeaseQueryData = 15,
        ActiveLeaseQuery = 16,
        LeaseQueryStatus = 17,
        Tls = 18
    };
}