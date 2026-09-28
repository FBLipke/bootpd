// CR_Start
///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2000 Intel Corporation.   All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////
// CR_End

#ifndef _IP_H
#define _IP_H


#define	IPLEN		4		/* length of an IP address */
#define	PROTUDP		17		/* IP package type is UDP */
#define	PROTIGMP	2		/* .. is IGMP */
#define	FR_DONT_FRAG	0x4000		/* .. don't fragment if set */
#define	FR_MORE_FRAG	0x2000		/* .. more fragments if set */
#define	FR_OFS		0x1fff		/* .. fragment offset mask */

/* Internet Protocol (IP) header */
typedef struct iph {

	UINT8	version; 		/* version and hdr length */
					/* each half is four bits */
	UINT8	service;		/* type of service for IP */
	UINT16	length,			/* total length of IP packet */
		ident,			/* transaction identification */
		frags;			/* combination of flags and value */

	UINT8	ttl,    		/* time to live */
		protocol;		/* higher level protocol type */

	UINT16	chksum;			/* header checksum */

	UINT8	source[IPLEN],		/* IP addresses */
		dest[IPLEN];

} IPLAYER;

struct in_addr {			/* INTERNET address */
	UINT32	s_addr;
};


#endif /* _IP_H */

/* EOF - $Workfile: ip.h $ */
