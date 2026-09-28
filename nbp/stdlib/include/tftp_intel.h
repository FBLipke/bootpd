// CR_Start
///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2000 Intel Corporation.   All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////
// CR_End

#ifndef _TFTP_H
#define _TFTP_H

/*
 *  TFTP.H - defines for the TFTP protocol 
 */


#define	TFTP_PORT	69		/* TFTP daemon port */
#define	TFTP_APORT	59		/* .. alternative TFTP daemon port */
#define	TFTP_PLEN1	512		/* .. regular package length */
#define	TFTP_PLEN2	1456		/* .. largest we support, based
					      on MTU 1500 and modulo 16 */

#define	MTFTP_SPORT	75		/* MTFTP default server port */
#define	MTFTP_CPORT	76		/* .. default client port */

#define	TFTP_RRQ	1		/* .. read request opcode */
#define	TFTP_WRQ	2		/* .. write request opcode */
#define	TFTP_DAT	3		/* .. data package opcode */
#define	TFTP_ACK	4		/* .. acknowledge opcode */
#define	TFTP_ERR	5		/* .. error package opcode */
#define	TFTP_OACK	6		/* .. option acknowledge package opcode */

#define	TFTP_OREAD	0x00		/* want to use TFTP for read */
#define	TFTP_OWRITE	0x01		/* .. for write */
#define	TFTP_OPCODE	0x01		/* .. opcode mask */
#define	TFTP_SILENT	0x02		/* .. transfer silently */

/* error codes */
#define	EUNDEF		0		/* not defined */
#define	ENOTFOUND	1		/* file not found */
#define	EACCESS		2		/* access violation */
#define	ENOSPACE	3		/* disk full or allocation exceeded */
#define	EBADOP		4		/* illegal TFTP operation */
#define	EBADID		5		/* unknown transfer ID */
#define	EEXISTS		6		/* file already exists */
#define	ENOUSER		7		/* no such user */
#define	EMCASTADDR	8		/* multicast address is not there for this file */

/* trivial file transfer protocol (TFTP) header */
struct tftph {
	UINT16	opcode;			/* operation code */
	UINT8	pack[TFTP_PLEN2+2];	/* tftp package */
};
typedef struct tftph	TFTPLAYER;


#endif /* _TFTP_H */

/* EOF - $Workfile: tftp.h $ */
