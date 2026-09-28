// CR_Start
///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2000 Intel Corporation.   All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////
// CR_End

#ifndef _PXEID_H
#define _PXEID_H


#ifndef __FAR
#define __FAR
#endif /* __FAR */

#ifndef __CDECL
#define __CDECL
#endif /* __CDECL */


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* PXE structure signatures
 */

#define BC_ROMID_SIG		"$BC$"
#define	UNDI_ROMID_SIG		"UNDI"
#define	BUSD_ROMID_SIG		"BUSD"

#define	PXE_SIG			"!PXE"
#define	PXENV_SIG		"PXENV+"

#define	BC_ROMID_REV		0x00
#define	UNDI_ROMID_REV		0x00
#define	BUSD_ROMID_REV		0x00

#define	PXE_REV			0x00
#define	PXENV_REV		0x0201


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* t_BC_ROMID
 *	PXE base-code ROM identification structure.
 *
 *	This structure must be located within the memory defined by the
 *	base-code option ROM header during IPL.  UNDI option ROMs that do
 *	not have their own base-code will check the 16bit word at offset
 *	16h in each option ROM header.  In the base-code option ROM this
 *	word is the offset of the start of the t_BC_ROMID structure.
 *
 *	The base-code loader routine is exptected to know the size of the
 *	code and data images that need to be copied to base memory.
 */

typedef struct s_BC_ROMID {
	UINT8 Signature[4];		/* Structure signature is not NULL */
					/* terminated. */

	UINT8 StructLength;		/* Length of this structure in */
					/* bytes. */

	UINT8 StructCksum;		/* Use to make byte checksum of this */
					/* structure == zero. */

	UINT8 StructRev;		/* Structure format revision number. */

	UINT8 BC_Rev[3];		/* API revision number stored in */
					/* Intel order. */
					/* Revision 2.1.0 == 0x00, 0x01, 0x02 */

	UINT16 BC_Loader;		/* Offset of base-code loader routine */
					/* in the option ROM image. */

	UINT16 StackSize;		/* Minimum stack segment size, in */
					/* bytes, needed to load and run the */
					/* base-code. */

	UINT16 DataSize;		/* Base-code runtime code and data */
	UINT16 CodeSize;		/* segment sizes. */
} t_BC_ROMID;


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
typedef struct s_UNDI_ROMID {
	UINT8 Signature[4];		/* Structure signature is not NULL */
					/* terminated. */

	UINT8 StructLength;		/* Length of this structure in */
					/* bytes. */

	UINT8 StructCksum;		/* Use to make byte checksum of this */
					/* structure == zero. */

	UINT8 StructRev;		/* Structure format revision number. */

	UINT8 UNDI_Rev[3];		/* API revision number stored in */
					/* Intel order. */
					/* Revision 2.1.0 == 0x00, 0x01, 0x02 */

	UINT16 UNDI_Loader;		/* Offset of UNDI loader routine */
					/* in the option ROM image. */

	UINT16 StackSize;		/* Minimum stack segment size, in */
					/* bytes, needed to load and run the */
					/* UNDI. */

	UINT16 DataSize;		/* UNDI runtime code and data */
	UINT16 CodeSize;		/* segment sizes. */

	UINT8 BusType[4];		/* 'ISAR', 'EISA', 'PCIR', 'PCCR' */
} t_UNDI_ROMID;


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
typedef struct s_PXE {
	UINT8 Signature[4];		/* Structure signature is not NULL */
					/* terminated. */

	UINT8 StructLength;		/* Length of this structure in */
					/* bytes. */

	UINT8 StructCksum;		/* Use to make byte checksum of this */
					/* structure == zero. */

	UINT8 StructRev;		/* Structure format revision number. */

	UINT8 reserved1;		/* must be zero */

	t_UNDI_ROMID __FAR *UNDI;	/* Far pointer to UNDI ROMID */

	t_BC_ROMID __FAR *Base;		/* Far pointer to base-code ROMID */

	UINT16 (__FAR __CDECL *EntryPointSP)(UINT16 func, void __FAR *param);
					/* 16bit stack segment API entry */
					/* point.  This will be seg:off in */
					/* real mode and sel:off in 16:16 */
					/* protected mode. */

	UINT16 (__FAR __CDECL *EntryPointESP)(UINT16 func, void __FAR *param);
					/* 32bit stack segment API entry */
					/* point.  This will be sel:off. */
					/* In real mode, sel == 0 */

	UINT16 (__FAR __CDECL *StatusCallout)(UINT16 param);
					/* Address of DHCP/TFTP status */
					/* callout routine. */

	UINT8 reserved2;		/* must be zero */

	UINT8 SegDescCnt;		/* Number of segment descriptors in */
					/* this structure. */

	UINT16 FirstSelector;		/* First segment descriptor in GDT */
					/* assigned to PXE. */

/*
	t_SEGDESC Stack;
	t_SEGDESC UNDIData;
	t_SEGDESC UNDICode;
	t_SEGDESC UNDICodeWrite;
	t_SEGDESC BC_Data;
	t_SEGDESC BC_Code;
	t_SEGDESC BC_CodeWrite;
*/
	t_NEWSEGDESC Stack;
	t_NEWSEGDESC UNDIData;
	t_NEWSEGDESC UNDICode;
	t_NEWSEGDESC UNDICodeWrite;
	t_NEWSEGDESC BC_Data;
	t_NEWSEGDESC BC_Code;
	t_NEWSEGDESC BC_CodeWrite;
} t_PXE;


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
typedef struct s_PXENV {
	char Signature[6];		/* "PXENV+" */
	UINT16 Version;			/* PXE version number.  LSB is minor */
					/* version.  MSB is major version. */
	UINT8 StructLength;		/* Length of PXE-2.0 Entry Point */
					/* structure in bytes. */
	UINT8 StructCksum;		/* Used to make structure checksum */
					/* equal zero. */
	UINT32 RMEntry;			/* Real mode API entry point  */
					/* segment:offset. */
	UINT16 PMEntryOff;		/* Protected mode API entry point */
	UINT32 PMEntrySeg;		/* segment:offset.  This will always */
					/* be zero.  Protected mode API calls */
					/* must be made through the API entry */
					/* points in the PXE Runtime ID */
					/* structure. */
	UINT16 StackSeg;		/* Real mode stack segment. */
	UINT16 StackSize;		/* Stack segment size in bytes. */
	UINT16 BaseCodeSeg;		/* Real mode base-code code segment. */
	UINT16 BaseCodeSize;		/* Base-code code segment size */
	UINT16 BaseDataSeg;		/* Real mode base-code data segment. */
	UINT16 BaseDataSize;		/* Base-code data segment size */
	UINT16 UNDIDataSeg;		/* Real mode UNDI data segment. */
	UINT16 UNDIDataSize;		/* UNDI data segment size in bytes. */
	UINT16 UNDICodeSeg;		/* Real mode UNDI code segment. */
	UINT16 UNDICodeSize;		/* UNDI code segment size in bytes. */
	t_PXE __FAR *RuntimePtr;	/* Real mode segment:offset pointer */
					/* to PXE Runtime ID structure. */
} t_PXENV;


/* EOF - $Workfile: pxeid.h $ */

#endif /* _PXEID_H */
