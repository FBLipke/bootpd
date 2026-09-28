// CR_Start
///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2000 Intel Corporation.   All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////
// CR_End

#ifndef _UDP_H
#define _UDP_H

/*
 *  UDP.H - defines for the UDP protocol 
 */


/* User Datagram Protocol (UDP) header */
struct udph {
	UINT16	source,			/* port numbers */
		dest;
	UINT16	length,			/* length of packet, including hdr */
		chksum;			/* TCP checksum of whole packet */
};
typedef struct udph	UDPLAYER;

struct phead {				/* pseudo header for UDP checksum */
	UINT32	source, dest;
	UINT8	pad, protocol;
	UINT16	length;
	UINT16	source_port,dest_port;			/* port numbers */
	UINT16	udp_length,			/* length of packet, including hdr */
		chksum;			/* UDP checksum of whole packet */
};
typedef struct phead	PHEAD;


#endif /* _UDP_H */

/* EOF - $Workfile: udp.h $ */
