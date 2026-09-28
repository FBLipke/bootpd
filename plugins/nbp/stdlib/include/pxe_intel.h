// CR_Start
///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2000 Intel Corporation.   All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////
// CR_End

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Sample PXE Constants for Extensions to DHCP Protocol */
/* All numbers are temporary for testing and subject to review */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef _PXE_H
#define _PXE_H

/* sample UDP port assigned to PXE/BINL */
#define PXE_BINL_PORT		4011
#define PXE_BINL_OLD_PORT	44776

#define PXE_CLS_CLIENT		"PXEClient"
#define PXE_CLS_SERVER		"PXEServer"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/*
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#define	PXE_PAD			0

/* Desc:   | opt | */
/* Offset: |   0 | */
/* Values: |   0 | */

typedef struct {
	UINT8 op;
} t_PXE_PAD;


#define	PXE_END			255

/* Desc:   | opt | */
/* Offset: |   0 | */
/* Values: | 255 | */

typedef struct {
	UINT8 op;
} t_PXE_END;


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Externally specified "PXEClient" class 43 options.
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#define PXE_MTFTP_IP		1	/* MTFTP IP address of bootfile */

/* Desc:   | opt | len |      IP address       | */
/* Offset: |   0 |   1 |   2 |   3 |   4 |   5 | */
/* Values: |   1 |   4 |   ? |   ? |   ? |   ? | */

typedef struct {
	UINT8 op;
	UINT8 len;
	UINT8 ip[4];
} t_PXE_MTFTP_IP;


#define PXE_MTFTP_CPORT		2	/* MTFTP client UDP port number */
					/* This is stored in Intel order, */
					/* not network order. */

/* Desc:   | opt | len | UDP port  | */
/* Offset: |   0 |   1 |   2 |   3 | */
/* Values: |   2 |   2 |   ? |   ? | */

typedef struct {
	UINT8 op;
	UINT8 len;
	UINT16 port;
} t_PXE_MTFTP_CPORT;


#define PXE_MTFTP_SPORT		3	/* MTFTP server UDP port number */
					/* This is stored in Intel order, */
					/* not network order. */

/* Desc:   | opt | len | UDP port  | */
/* Offset: |   0 |   1 |   2 |   3 | */
/* Values: |   3 |   2 |   ? |   ? | */

typedef struct {
	UINT8 op;
	UINT8 len;
	UINT16 port;
} t_PXE_MTFTP_SPORT;


#define PXE_MTFTP_TMOUT		4	/* MTFTP start delay (in seconds) */

/* Desc:   | opt | len | sec | */
/* Offset: |   0 |   1 |   2 | */
/* Values: |   4 |   1 |   ? | */


typedef struct {
	UINT8 op;
	UINT8 len;
	UINT8 tmout;
} t_PXE_MTFTP_TMOUT;


#define PXE_MTFTP_DELAY		5	/* MTFTP re-open delay (in seconds) */

/* Desc:   | opt | len | sec | */
/* Offset: |   0 |   1 |   2 | */
/* Values: |   5 |   1 |   ? | */


typedef struct {
	UINT8 op;
	UINT8 len;
	UINT8 delay;
} t_PXE_MTFTP_DELAY;


#define PXE_DISCOVERY_CONTROL	6
# define PXE_DISCOVERY_CONTROL_BCAST_DISABLE	0x01
# define PXE_DISCOVERY_CONTROL_MCAST_DISABLE	0x02
# define PXE_DISCOVERY_CONTROL_SRVLIST_ONLY	0x04
# define PXE_DISCOVERY_CONTROL_BSTRAP_OVERRIDE	0x08

/* Desc:   | opt | len | flag | */
/* Offset: |   0 |   1 |    2 | */
/* Values: |   6 |   1 |    ? | */


typedef struct {
	UINT8 op;
	UINT8 len;
	UINT8 flag;
} t_PXE_DISCOVERY_CONTROL;


#define PXE_MCAST_DISCOVERY_ADDR	7	/* Multicast discovery */
						/* IP address. */

/* Desc:   | opt | len |   Discovery IP Addr   | */
/* Offset: |   0 |   1 |   2 |   3 |   4 |   5 | */
/* Values: |   3 |   2 |   ? |   ? |   ? |   ? | */


typedef struct {
	UINT8 op;
	UINT8 len;
	UINT8 ip[4];
} t_PXE_MCAST_DISCOVERY_ADDR;


#define PXE_BOOT_SERVERS	8

/* Desc:   | opt | len | srv-type  |ipcnt|     ip-list     | */
/* Offset: |   0 |   1 |   2 |   3 |   4 |   5 |   6 |   7 | */
/* Values: |  68 |   N |   ? |   ? |   ? |   ? |   ? |   ? | */

typedef struct {
	UINT8 op;
	UINT8 len;
	UINT8 data[1];
} t_PXE_BOOT_SERVERS;


#define PXE_BOOT_MENU		9

/* Desc:   | opt | len | srv-type  |d-len| description | */
/* Offset: |   0 |   1 |   2 |   3 |   4 |   5 | ..... | */
/* Values: |  68 |   N |   ? |   ? |   ? |   ? |       | */

typedef struct {
	UINT8 op;
	UINT8 len;
	UINT8 data[1];
} t_PXE_BOOT_MENU;


#define PXE_BOOT_PROMPT		10
# define PXE_BOOT_PROMPT_AUTO_SELECT	0
# define PXE_BOOT_PROMPT_NO_TIMEOUT	255

/* Desc:   | opt | len |   Dur   |   Prompt   | */
/* Offset: |   0 |   1 |    2    |   3 - N+1  | */
/* Values: |  68 |   N | 0 - 255 | "prompt"\0 | */

typedef struct {
	UINT8 op;
	UINT8 len;
	UINT8 dur;
	char prompt[1];
} t_PXE_BOOT_PROMPT;


#define PXE_MCAST_ADDRS_ALLOC	11	/* */

/* Desc:   | opt | len | MTFTP Base Address    | t-num     | m-num     | */
/* Offset: |   0 |   1 |   2 |   3 |   4 |   5 |   6 |   7 |   8 |   9 | */
/* Values: |   3 |   2 |   ? |   ? |   ? |   ? |   ? |   ? |   ? |   ? | */

typedef struct {
	UINT8 op;
	UINT8 len;
	UINT8 ip[4];
	UINT16 t_num;
	UINT16 m_num;
} t_PXE_MCAST_ADDRS_ALLOC;


#define	PXE_CREDENTIAL_TYPES		12		/* Credential Types */

typedef struct {
	UINT8 op;
	UINT8 len;
	UINT32 type[1];			/* network order */
} t_PXE_CREDENTIAL_TYPES;


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Loader (BSTRAP.1/MAN.2) options.  Intel vendor specific.
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#define	PXE_NIC_PATH		64	/* NIC path on LCM */

/* Desc:   | opt | len | "NIC path" | */
/* Offset: |   0 |   1 | 2 ... N+1  | */
/* Values: |  64 |   N | "????",\0  | */


#define	PXE_MAN_INFO		65	/* Management information */

/* Desc:   | opt | len | #ip | IPadr | Base Name | Menu Text | */
/* Offset: |   0 |   1 |   2 | 3 - 6 | 7   -   I | I+1 - N+1 | */
/* Values: |  65 |   N |   ? | ? - ? | "name",\0 | "text",\0 | */


#define	PXE_OS_INFO		66	/* Networkl OS information */

/* Desc:   | opt | len | #ip | IPadr | Base Name | Menu Text | */
/* Offset: |   0 |   1 |   2 | 3 - 6 | 7   -   I | I+1 - N+1 | */
/* Values: |  66 |   N |   ? | ? - ? | "name",\0 | "text",\0 | */


#define	PXE_BOOT_OS_INFO	67	/* Networkl OS information */

/* Desc:   | opt | len | #ip | IPadr | Base Name | Menu Text | */
/* Offset: |   0 |   1 |   2 | 3 - 6 | 7   -   I | I+1 - N+1 | */
/* Values: |  67 |   N |   ? | ? - ? | "name",\0 | "text",\0 | */

#define	PXE_PROMPT_INFO	68	/* Prompt display information */

/* Desc:   | opt | len |   Dur   |   Prompt   | */
/* Offset: |   0 |   1 |    2    |   3 - N+1  | */
/* Values: |  68 |   N | 0 - 255 | "prompt"\0 | */

#define	PXE_OS_INFO2	69	/* Networkl OS information */

/* Desc:   | opt | len | ord | SrvIP | #ip | IPadr | Base Name | Menu Text | */
/* Offset: |   0 |   1 |  2  | 3 - 6 |  7  | 8 - I |  I+1 - M  | M+1 - N+1 | */
/* Values: |  69 |   N |  ?  | ? - ? |  ?  | ? - ? | "name",\0 | "text",\0 | */

#define	PXE_BOOT_OS_INFO2	70	/* Networkl OS information */

/* Desc:   | opt | len | ord | SrvIP | #ip | IPadr | Base Name | Menu Text | */
/* Offset: |   0 |   1 |  2  | 3 - 6 |  7  | 8 - I |  I+1 - M  | M+1 - N+1 | */
/* Values: |  70 |   N |  ?  | ? - ? |  ?  | ? - ? | "name",\0 | "text",\0 | */

#define	PXE_BOOT_ITEM		71

typedef struct {
	UINT8 op;
	UINT8 len;
	UINT16 type;			/* network order */
	UINT16 layer;			/* network order */
} t_PXE_BOOT_ITEM;

/* REMOVED PER REQUEST OF PLATINUM TEAM
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Vendor sub-options.  Intel vendor specific.
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/*#define PXE_LCMSERVER_TAG	179	/* Option tag for Server. */

/* Desc:   | opt | len | server name */
/* Offset: |   0 |   1 |      2       */
/* Values: | 179 |   N | null-terminated NT computer name of LCM */


/*#define PXE_LCMDOMAIN_TAG	180	/* Option tag for Domain. */

/* Desc:   | opt | len | domain name */
/* Offset: |   0 |   1 |      2      */
/* Values: | 180 |   N | null-terminated string */
/* Data: NT domain name of LCM */


/*#define PXE_LCMNICOPT0_TAG	181	/* Option tag for NIC option 0. */

/* Desc:   | opt | len | unknown */
/* Offset: |   0 |   1 | unknown */
/* Values: | 181 |   ? | unknown */


/*#define PXE_LCMWRKGRP_TAG	190	/* Option tag for workgroup. */

/* Desc:   | opt | len | workgroup name */
/* Offset: |   0 |   1 |      2         */
/* Values: | 190 |   N | null-terminated string */
/* Data: NT workgroup name of LCM */

/*#define PXE_DISCOVERY_TAG	191	/* Option tag to specify this is a discovery message. */

/* Desc:   | opt | len | discovery */
/* Offset: |   0 |   1 |     2     */
/* Values: | 191 |   1 |    0,1    */
/* Data: 1 if this is a discovery message (assumed 0 if not present) */

/*#define PXE_CONFIGURED_TAG  192 /* Option tag to tell whether HH is configured or not. */

/* Desc:   | opt | len | configured */
/* Offset: |   0 |   1 |     2      */
/* Values: | 192 |   1 |    0,1     */
/* Data: 1 for configured HH, 0 for unconfigured HH */

/*#define PXE_LCMVERSION_TAG	193	/* Option tag to specify LCM version. */

/* Desc:   | opt | len | version */
/* Offset: |   0 |   1 |    2    */
/* Values: | 193 |   4 |  DWORD  */
/* Data: long (network order) containing LCM version number */

/*#define PXE_LCMSERIALNO_TAG	194	/* Option tag to specify LCM version. */

/* Desc:   | opt | len | serial number */
/* Offset: |   0 |   1 |       2       */
/* Values: | 194 |   N | null-terminated string */
/* Data: LCM serial number as ASCII string */

#endif /* _PXE_H */

/* EOF - $Workfile: pxe.h $ */
