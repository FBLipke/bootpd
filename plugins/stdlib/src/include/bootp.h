// CR_Start
///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2000 Intel Corporation.   All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////
// CR_End

#ifndef __BOOTP_H
#define __BOOTP_H


#include "ip.h"

#define BOOTP_VENDOR    64      /* BOOTP standard vendor field size */

#if 1
#define BOOTP_DHCPVEND  1024    /* DHCP extended vendor field size */
#else
#define BOOTP_DHCPVEND  312	/* DHCP standard vendor field size */
#endif

/* BOOTstrap Protocol (BOOTP) header */
typedef struct bootph {
	UINT8   opcode,                 /* operation code */
		hardware,               /* hardware type */
		hardlen,                /* length of hardware address */
		gatehops;               /* gateways hops */
	UINT32  ident;                  /* transaction identification */
	UINT16  seconds,                /* seconds elapsed since boot began */
		flags;                  /* flags */
	UINT8   cip[IPLEN],             /* client IP address */
		yip[IPLEN],             /* your IP address */
		sip[IPLEN],             /* server IP address */
		gip[IPLEN];             /* gateway IP address */
	UINT8   caddr[16],              /* client hardware address */
		sname[64],              /* server name */
		bootfile[128];          /* bootfile name */
	union {
		UINT8   d[BOOTP_DHCPVEND];      /* vendor-specific stuff */
		struct {
			UINT8   magic[4];       /* magic number */
			UINT32  flags;          /* flags/opcodes etc */
			UINT8   pad[56];        /* padding chars */
		} v;
	} vendor;
} BOOTPLAYER;

#define VM_RFC1048      0x63825363L     /* RFC1048 magic number */

#define BOOTP_SPORT     67              /* BOOTP server port */
#define BOOTP_CPORT     68              /* .. client port */

#define BOOTP_REQ       1               /* BOOTP request */
#define BOOTP_REP       2               /* .. reply */

/* BOOTP flags field */
#define BOOTP_BCAST     0x8000          /* BOOTP broadcast flag */
#define BOOTP_FLAGS     BOOTP_BCAST     /* .. for FDDI address transl. */


#endif /* __BOOTP_H */

/* EOF - $Workfile: bootp.h $ */
