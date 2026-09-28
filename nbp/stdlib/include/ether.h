// CR_Start
///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2000 Intel Corporation.   All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////
// CR_End

#ifndef _ETHER_H
#define _ETHER_H

/*
 *  ETHER.H - defines for Ethernet protocol
 */


#define	ADDR_LEN	16		/* length of  media address */
#define	DADDLEN		6		/* length of an Ethernet address */
#define	EIP		0x0800		/* Ethernet package type is IP */
#define	EARP		0x0806		/* .. package type is ARP */
#define	MCAST		0x01005e00L	/* Ethernet multicast header */

#define	ACTRL		0x10		/* Token Ring token bit set */
#define	FCTRL		0x40		/* .. frame type 1 */

#define	CTRL_UI		0x03		/* IEEE unnumbered frame */
#define	CTRL_XID	0xaf		/* .. exchange ID */
#define	CTRL_TEST	0xd3		/* .. test */
#define	CTRL_POLL	0x10		/* .. poll bit */


#ifdef AVL
/* raw ethernet (ET) header */
struct eth {
	UINT8	dest[DADDLEN],		/* destination address */
		source[DADDLEN];	/* source address */
	UINT16	type;			/* Ethernet packet type  */
};
typedef struct eth	ETLAYER;
#endif	/* AVL */

struct media {	
	UINT8	prot_type;		/* unknown, IP, ARP, RARP */
	UINT8	flag;			/* directed, broadcast, multicast  */
};
typedef struct media	MLAYER;


#endif /* _ETHER_H */

/* EOF - $Workfile: ether.h $ */
