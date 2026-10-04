
#include "../Bootpd.h"
#pragma once
namespace bootp::Plugins::DHCP
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

    enum class HardwareType
    {
        Reserved = 0,
        Ethernet = 1,
        Experimental_ethernet = 2,
        Ax25 = 3,
        Pronet_token_ring = 4,
        Chaos = 5,
        Ieee802 = 6,
        Arcnet = 7,
        Hyperchannel = 8,
        Lanstar = 9,
        Autonet_short_addr = 10,
        Localtalk = 11,
        Localnet = 12,
        Ultralink = 13,
        Smds = 14,
        Frame_relay = 15,
        Atm_16 = 16,
        Hdlc = 17,
        Fibre_channel = 18,
        Atm_19 = 19,
        Serial_line = 20,
        Atm_21 = 21,
        Mil_std_188_220 = 22,
        Metricom = 23,
        Ieee1394 = 24,
        Mapos = 25,
        Twinaxial = 26,
        Eui64 = 27,
        Hiparp = 28,
        Iso7816_3 = 29,
        Arpsec = 30,
        Ipsec_tunnel = 31,
        Infiniband = 32,
        Tia_102_p25_cai = 33,
        Wiegand = 34,
        Pure_ip = 35,
        Hw_exp1 = 36,
        Hfi = 37,
        Unified_bus = 38,
        Hw_exp2 = 256,
        Aethernet = 257,
        Reserved_65535 = 65535
    };
}
