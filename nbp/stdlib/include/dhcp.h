/**
 * DHCP Options List
 * RFC 2132, RFC 4578, Intel LCM
 */

#ifndef _DHCP_H
#define _DHCP_H

/* DHCP Options */
#define DHCP_OPT_PAD             0
#define DHCP_OPT_SUBNET_MASK     1
#define DHCP_OPT_ROUTER          3
#define DHCP_OPT_DNS             6
#define DHCP_OPT_LOG_SERVER      7
#define DHCP_OPT_LPR_SERVER      9
#define DHCP_OPT_HOSTNAME        12
#define DHCP_OPT_BOOTFILE_LEN    13
#define DHCP_OPT_DOMAIN_NAME     15
#define DHCP_OPT_ROOT_PATH       17
#define DHCP_OPT_EXTENDED_ROOT_PATH 18
#define DHCP_OPT_SWAP_SERVER     16
#define DHCP_OPT_BROADCAST       20
#define DHCP_OPT_RESERVED_21     21
#define DHCP_OPT_RESERVED_22     22
#define DHCP_OPT_RESERVED_23     23
#define DHCP_OPT_PATH_MTU_PLATEAU 25
#define DHCP_OPT_INTERFACE_MTU   26
#define DHCP_OPT_ALL_SUBNETS_LOCAL 27
#define DHCP_OPT_BROADCAST_ADDR  28
#define DHCP_OPT_MASK_DISCOVERY  29
#define DHCP_OPT_MASK_SUPPLIER   30
#define DHCP_OPT_ROUTER_DISCOVERY 31
#define DHCP_OPT_ROUTER_SOLICIT  32
#define DHCP_OPT_STATIC_ROUTES   33
#define DHCP_OPT_TRAILER_ENCAPS  34
#define DHCP_OPT_ARP_CACHE_TIMEOUT 35
#define DHCP_OPT_ETHER_ENCAPS    36
#define DHCP_OPT_TCP_DEFAULT_TTL 37
#define DHCP_OPT_TCP_KEEPALIVE_INT 38
#define DHCP_OPT_TCP_KEEPALIVE_GARBAGE 39
#define DHCP_OPT_NIS_DOMAIN      40
#define DHCP_OPT_NIS_SERVERS     41
#define DHCP_OPT_NTP_SERVERS     42
#define DHCP_OPT_VENDOROPTS 43
#define DHCP_OPT_NETBIOS_NS      44
#define DHCP_OPT_NETBIOS_DD      45
#define DHCP_OPT_NETBIOS_NODE_T  46
#define DHCP_OPT_NETBIOS_SCOPE   47
#define DHCP_OPT_X_FONT_SERVER   48
#define DHCP_OPT_X_DISPLAY_MGR   49
#define DHCP_OPT_RESERVED_64     64
#define DHCP_OPT_RESERVED_65     65
#define DHCP_OPT_NIS_PLUS_DOMAIN 64
#define DHCP_OPT_NIS_PLUS_SERVERS 65
#define DHCP_OPT_TFTP_SERVER     66
#define DHCP_OPT_BOOTFILE        67
#define DHCP_OPT_MAX_DHCP_SIZE   57
#define DHCP_OPT_CLASS_IDENTIFIER 60
#define DHCP_OPT_VCI             60      /* Vendor Class Identifier */
#define DHCP_OPT_CLIENT_IDENTIFIER 61
#define DHCP_OPT_RESERVED_62     62
#define DHCP_OPT_PAD64           64
#define DHCP_OPT_PAD65           65
#define DHCP_OPT_CLIENT_FQDN     81
#define DHCP_OPT_DELAYED_ACK     83
#define DHCP_OPT_VSS             82      /* Virtual Subnet Selection */
#define DHCP_OPT_SERVER_IDENTIFIER 54
#define DHCP_OPT_MESSAGE_TYPE     53
#define DHCP_OPT_SERVER_NAME      66
#define DHCP_OPT_BOOTFILE_NAME   67
#define DHCP_OPT_MAX_MESSAGE_SIZE 57
#define DHCP_OPT_RENEWAL_TIME    58
#define DHCP_OPT_REBINDING_TIME   59
#define DHCP_OPT_LEASE_TIME       51
#define DHCP_OPT_END             255

/* PXE Options (RFC 4578) - in Option 43 as sub-options */
#define PXE_OPT_BOOT_SERVER      8
#define PXE_OPT_BOOT_ITEM        71
#define PXE_OPT_CREDENTIAL_TYPES 12
#define PXE_OPT_BOOT_SERVERS     8
#define PXE_OPT_BOOT_MENU       9
#define PXE_OPT_MENU_PROMPT      10
#define PXE_OPT_MULTICAST_ADDR   11

/* WDS Options - in Option 43 */
#define WDS_OPT_MESSAGE          6
#define WDS_OPT_NEXT_ACTION      2
#define WDS_OPT_REQUEST_ID        5

/* Intel LCM Options (128-135) - RFC 4578 */
#define LCM_BOOT_RECOVERY_ATTEMPT_COUNT  128
#define LCM_BOOT_MANIFEST_ID             129
#define LCM_BOOT_RECOVERY_INVOCATION_POL 130
#define LCM_BOOT_RECOVERY_RETRY_DELAY     131
#define LCM_BOOT_MANIFEST_URI             132

/* PXE Architecture Types (Option 139) */
#define ARCH_INTEL_X86          0x0000
#define ARCH_NEC98               0x0001
#define ARCH_ALPHA               0x0002
#define ARCH_ARM                 0x0003
#define ARCH_MIPS_LE             0x0004
#define ARCH_MIPS_BE             0x0005
#define ARCH_ARM64               0x0006
#define ARCH_x86_64_UEFI        0x0007
#define ARCH_ARM32_UEFI         0x0008
#define ARCH_AARCH64_UEFI       0x0009
#define ARCH_RISCV64_UEFI       0x000A

/* DHCP Message Types */
#define DHCP_DISCOVER            1
#define DHCP_OFFER               2
#define DHCP_REQUEST             3
#define DHCP_DECLINE             4
#define DHCP_ACK                5
#define DHCP_NAK                6
#define DHCP_RELEASE            7
#define DHCP_INFORM             8

#endif /* _DHCP_H */

/* WDS Actions - Windows Deployment Services */
#define WDS_APPROVAL           1
#define WDS_REFERRAL           3
#define WDS_ABORT              5
