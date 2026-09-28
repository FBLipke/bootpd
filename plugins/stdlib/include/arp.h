// CR_Start
///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2000 Intel Corporation.   All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////
// CR_End

#ifndef _ARP_H
#define _ARP_H

/*
 * ARP.H - defines for the ARP protocol 
 */


/* Address Resolution Protocol (ARP) header */
struct arph {
#ifdef AVL
	ETLAYER		e;		/* ethernet layer */
#endif	/* AVL */
	MLAYER		m;		/* Media layer */
	UINT16		hardware,	/* hardware type */
			protocol;	/* protocol type */
	UINT8		hardlen,	/* length of hardware address */
			protlen;	/* length of protocol */
	UINT16		opcode;		/* operation code */
	UINT8		sha[DADDLEN],	/* hardware address of sender */
			spa[IPLEN],	/* protocol address of sender */
			tha[DADDLEN],	/* hardware address of target */
			tpa[IPLEN];	/* protocol address of target */
};
typedef struct arph	ARPBUF;

#define	ARP_REQ		1	/* ARP request */
#define	ARP_REP		2	/* .. reply */
#define	ARP_RREQ	3	/* RARP request */
#define	ARP_RREP	4	/* .. reply */

#ifdef AVL
 #define	ARP_ETHER	1	/* .. ethernet hardware */
 #define	ARP_IEEE	6	/* .. IEEE 802 hardware */
#endif	/* AVL */

#define	ARP_PROT	EIP	/* .. protocol type is IP */


#endif /* _ARP_H */

/* EOF - $Workfile: arp.h $ */
