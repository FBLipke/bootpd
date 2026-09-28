// CR_Start
///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2000 Intel Corporation.   All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////
// CR_End

#ifndef _PXENV_CMN_H
#define _PXENV_CMN_H

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* PXENV.H - PXENV/TFTP/UNDI API common, Version 2.x, 97-Jan-17
 *
 * Constant and type definitions used in other PXENV API header files.
 */


/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* Storage types.
 */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Simple types
 */
typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned long UINT32;
typedef signed char INT8;
typedef signed short INT16;
typedef signed long INT32;

typedef UINT16 SEGSEL;			/* Real mode segment or protected */
					/* mode selector. */

typedef UINT16 OFF16;			/* Unsigned 16bit offset. */


typedef UINT32 ADDR32;


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
typedef UINT16 PXENV_EXIT;

typedef UINT16 PXENV_STATUS;


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
typedef struct s_segoff16 {
	OFF16 offset;
	SEGSEL segment;
} SEGOFF16;


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
typedef struct s_SEGDESC {
	UINT16 LimitLow;
	UINT16 BaseLow;
	UINT8 BaseMid;
	UINT8 Type;
	UINT8 LimitHigh;
	UINT8 BaseHigh;
} t_SEGDESC;

typedef struct s_NEWSEGDESC {
	UINT16 Seg_Addr;
	UINT32 Phy_Addr;
	UINT16 Seg_Size;
} t_NEWSEGDESC;


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
typedef UINT16 UDP_PORT;


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define MAC_ADDR_LEN		16

typedef UINT8 MAC_ADDR[MAC_ADDR_LEN];


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Loader & BUSD parameter structures.
 */
typedef struct s_BC_Loader {
	UINT16	Status;	
	UINT16	_AX_;	
	UINT16	_BX_;	
	UINT16	_DX_;	
	UINT16	_DI_;	
	UINT16	_ES_;	
	UINT16	UNDI_ROMID_Off;	
	UINT16	UNDI_ROMID_Seg;	
} t_BC_Loader;

typedef struct s_UNDI_Loader {
	UINT16	Status;	
	UINT16	_AX_;	
	UINT16	_BX_;	
	UINT16	_DX_;	
	UINT16	_DI_;	
	UINT16	_ES_;	
	UINT16	UNDI_DS;	
	UINT16	UNDI_CS;	
	SEGOFF16 PXEptr;
	SEGOFF16 PXENVptr;
} t_UNDI_Loader;

typedef struct s_BUSD_Enable {
	UINT16	Status;	
	UINT16	_AX_;	
	UINT16	_BX_;	
	UINT16	_DX_;	
	UINT16	_DI_;	
	UINT16	_ES_;	
	UINT16	UNDI_ROMID_Off;	
	UINT16	UNDI_ROMID_Seg;	
} t_BUSD_Enable;

typedef struct s_BUSD_Disable {
	UINT16	Status;	
	UINT16	_AX_;	
	UINT16	_BX_;	
	UINT16	_DX_;	
	UINT16	_DI_;	
	UINT16	_ES_;	
	UINT16	UNDI_ROMID_Off;	
	UINT16	UNDI_ROMID_Seg;	
} t_BUSD_Disable;


/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* CPU types
 */
#define	PXENV_CPU_X86		0
#define	PXENV_CPU_NECPC98	1
#define	PXENV_CPU_IA64		2
#define	PXENV_CPU_ALPHA		3
#define PXENV_CPU_ARCX86	4
#define	PXENV_CPU_PPC		5


/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* Bus types
 */
#define	PXENV_BUS_ISA		0
#define	PXENV_BUS_EISA		1
#define	PXENV_BUS_MCA		2
#define	PXENV_BUS_PCI		3
#define	PXENV_BUS_VESA		4
#define	PXENV_BUS_PCMCIA	5


/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* Result codes returned in AX by a PXENV API service.
 */
#define PXENV_EXIT_SUCCESS	0x0000
#define PXENV_EXIT_FAILURE	0x0001


/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* Status codes returned in the status word of PXENV API parameter structures.
 */

/* Generic API errors - these do not match up with the M0x or E0x messages
 * that are reported by the loader.
 */
#define PXENV_STATUS_SUCCESS				0x00
#define	PXENV_STATUS_FAILURE				0x01
#define	PXENV_STATUS_BAD_FUNC				0x02
#define	PXENV_STATUS_UNSUPPORTED			0x03
#define PXENV_STATUS_KEEP_UNDI				0x04
#define PXENV_STATUS_KEEP_ALL				0x05

#if 0
/* Initialization routine errors (0x00 to 0x0F) */
#define PXENV_STATUS_INIT_
#endif

/* ARP errors (0x10 to 0x17) */
#define	PXENV_STATUS_ARP_TIMEOUT			0x11

/* Base-Code state errors */
#define PXENV_STATUS_UDP_CLOSED				0x18
#define PXENV_STATUS_UDP_OPEN				0x19
#define	PXENV_STATUS_TFTP_CLOSED			0x1A
#define PXENV_STATUS_TFTP_OPEN				0x1B

/* BIOS/system errors (0x20 to 0x2F) */
#define	PXENV_STATUS_MCOPY_PROBLEM			0x20
#define PXENV_STATUS_BIS_INTEGRITY_FAILURE		0x21
#define PXENV_STATUS_BIS_VALIDATE_FAILURE		0x22
#define PXENV_STATUS_BIS_INIT_FAILURE			0x23
#define PXENV_STATUS_BIS_SHUTDOWN_FAILURE		0x24
#define PXENV_STATUS_BIS_GBOA_FAILURE			0x25
#define PXENV_STATUS_BIS_FREE_FAILURE			0x26
#define PXENV_STATUS_BIS_GSI_FAILURE			0x27
#define PXENV_STATUS_BIS_BAD_CKSUM			0x28

/* (M)TFTP errors (0x30 to 0x4F) */
#define	PXENV_STATUS_TFTP_CANNOT_ARP_ADDRESS		0x30
#define	PXENV_STATUS_TFTP_OPEN_TIMEOUT			0x32
#define	PXENV_STATUS_TFTP_UNKNOWN_OPCODE		0x33
#define	PXENV_STATUS_TFTP_READ_TIMEOUT			0x35
#define	PXENV_STATUS_TFTP_ERROR_OPCODE			0x36
#define	PXENV_STATUS_TFTP_CANNOT_OPEN_CONNECTION	0x38
#define	PXENV_STATUS_TFTP_CANNOT_READ_FROM_CONNECTION	0x39
#define	PXENV_STATUS_TFTP_TOO_MANY_PACKAGES		0x3A
#define	PXENV_STATUS_TFTP_FILE_NOT_FOUND		0x3B
#define	PXENV_STATUS_TFTP_ACCESS_VIOLATION		0x3C
#define	PXENV_STATUS_TFTP_NO_MCAST_ADDRESS		0x3D
#define PXENV_STATUS_TFTP_NO_FILESIZE			0x3E
#define	PXENV_STATUS_TFTP_INVALID_PACKET_SIZE		0x3F

/* reserved errors (0x40 to 0x4F) */

/* BOOTP/DHCP errors (0x50 to 0x5F) */
#define PXENV_STATUS_DHCP_TIMEOUT			0x51
#define PXENV_STATUS_DHCP_NO_IP_ADDRESS          	0x52
#define	PXENV_STATUS_DHCP_NO_BOOTFILE_NAME       	0x53
#define PXENV_STATUS_DHCP_BAD_IP_ADDRESS          	0x54

/* Driver errors (0x60 to 0x6F) */
/* These errors are for UNDI compatible NIC drivers. */
#define PXENV_STATUS_UNDI_INVALID_FUNCTION		0x60
#define PXENV_STATUS_UNDI_MEDIATEST_FAILED 		0x61
#define	PXENV_STATUS_UNDI_CANNOT_INIT_NIC_FOR_MCAST	0x62
#define PXENV_STATUS_UNDI_CANNOT_INITIALIZE_NIC		0x63
#define PXENV_STATUS_UNDI_CANNOT_INITIALIZE_PHY		0x64
#define PXENV_STATUS_UNDI_CANNOT_READ_CONFIG_DATA	0x65
#define PXENV_STATUS_UNDI_CANNOT_READ_INIT_DATA		0x66
#define PXENV_STATUS_UNDI_BAD_MAC_ADDR 			0x67
#define PXENV_STATUS_UNDI_BAD_EEPROM_CKSUM 		0x68
#define PXENV_STATUS_UNDI_ERROR_SETTING_ISR		0x69
#define PXENV_STATUS_UNDI_INVALID_STATE			0x6A

/* Bootstrap (.1) errors (0x70 to 0x7F) */
/* These errors are for the bootstrap layer. */
#define PXENV_STATUS_BSTRAP_PROMPT_MENU			0x74
#define PXENV_STATUS_BSTRAP_MCAST_ADDR			0x76
#define PXENV_STATUS_BSTRAP_MISSING_LIST		0x77
#define PXENV_STATUS_BSTRAP_NO_RESPONSE			0x78
#define PXENV_STATUS_BSTRAP_FILE_TOO_BIG		0x79

/* Environment (.2) errors (0x80 to 0x8F) */
/* These errors are for environment layers. */

/* reserved errors (0x90 to 0x9F) */

/* Misc errors (0xA0 to 0xAF) */
#define	PXENV_STATUS_CANCELED_BY_KEYSTROKE 		0xA0
#define	PXENV_STATUS_BINL_NO_PXE_SERVER         	0xA1
#define	PXENV_STATUS_NOT_AVAILABLE_IN_PMODE     	0xA2
#define	PXENV_STATUS_NOT_AVAILABLE_IN_RMODE     	0xA3

/* BUSD errors (0xB0 to 0xBF) */
#define PXENV_STATUS_BUSD_BUS_NOT_ENABLED		0xB0
#define PXENV_STATUS_BUSD_DEVICE_NOT_ENABLED		0xB1
#define PXENV_STATUS_BUSD_NO_ROMID			0xB2
#define PXENV_STATUS_BUSD_BAD_ROMID			0xB3

/* BC/UNDI Loader errors (0xC0 to 0xCF) */
#define PXENV_STATUS_LOADER_NO_FREE_BASE_MEMORY		0xC0
#define PXENV_STATUS_LOADER_NO_BC_ROMID			0xC1
#define PXENV_STATUS_LOADER_BAD_BC_ROMID		0xC2
#define PXENV_STATUS_LOADER_BAD_BC_RUNTIME_IMAGE	0xC3
#define PXENV_STATUS_LOADER_NO_UNDI_ROMID		0xC4
#define PXENV_STATUS_LOADER_BAD_UNDI_ROMID		0xC5
#define PXENV_STATUS_LOADER_BAD_UNDI_DRIVER_IMAGE	0xC6
#define PXENV_STATUS_LOADER_NO_PXE_STRUCT		0xC8
#define PXENV_STATUS_LOADER_NO_PXENV_STRUCT		0xC9
#define PXENV_STATUS_LOADER_UNDI_START			0xCA
#define PXENV_STATUS_LOADER_BC_START			0xCB

/* Vendor errors (0xD0 to 0xFF) */

#endif /* _PXENV_CMN_H */

/* EOF - $Workfile: pxe_cmn.h $ */
