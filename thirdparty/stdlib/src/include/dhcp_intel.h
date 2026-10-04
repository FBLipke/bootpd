// CR_Start
///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2000 Intel Corporation.   All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////
// CR_End


#ifndef _DHCP_H
#define _DHCP_H

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define IP_ADDR_LEN		4

typedef union s_ip4 {
	UINT32 num;
	UINT8 array[IP_ADDR_LEN];
} IP4;


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define DHCP_PAD	0	/* pad character */


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define DHCP_SUBNET	1	/* subnet mask */

typedef struct {
	UINT8 opt;
	UINT8 len;
	IP4 ip;
} t_DHCP_SUBNET;


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define DHCP_TOFFSET	2	/* time offset */


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define DHCP_ROUTER	3	/* router option */

typedef struct {
	UINT8 opt;
	UINT8 len;
	IP4 ip[1];		/* array of router IP addresses */
} t_DHCP_ROUTER;


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define DHCP_TSRV	4	/* time server */


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define DHCP_NSRV	5	/* name server */


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define DHCP_DNSRV	6	/* domain name server */


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define DHCP_LSRV	7	/* log server */


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define DHCP_CSRV	8	/* cookie server */


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define DHCP_HNAME	12	/* hostname */


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define	DHCP_BFSIZE	13	/* Boot File Size */


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define DHCP_DNAME	15	/* domainname */


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define DHCP_VENDOR	43	/* vendor specific */

typedef struct {
	UINT8 opt;
	UINT8 len;
	UINT8 buf[1];
} t_DHCP_VENDOR;


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define DHCP_NBNS	44	/* NetBIOS/WINS name server */


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define DHCP_NBDD	45	/* NetBIOS datagram distribution server */


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define DHCP_NBNODE	46	/* NetBIOS node type */


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define DHCP_NBSCOPE	47	/* NetBIOS scope */


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define DHCP_REQIP	50	/* requested IP address */

typedef struct {
	UINT8 opt;
	UINT8 len;
	IP4 ip;
} t_DHCP_REQIP;


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define DHCP_LEASE	51	/* IP addr lease time */


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define DHCP_OVRLOAD	52	/* option overload */

typedef struct {
	UINT8 opt;
	UINT8 len;
	UINT8 type;
} t_DHCP_OVRLOAD;


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define DHCP_MSGTYPE	53	/* DHCP message type opcode */
	#define DHCP_DISCOVER	1	/* DHCP discover packet */
	#define DHCP_OFFER	2	/* DHCP offer packet */
	#define DHCP_REQUEST	3	/* DHCP request packet */
	#define DHCP_DECLINE	4	/* DHCP invalid config */
	#define DHCP_ACK	5	/* DHCP acknowledge packet */
	#define DHCP_NAK	6	/* DHCP negative ack packet */
	#define DHCP_RELEASE	7	/* DHCP release IP addr */
	#define DHCP_INFORM	8	/* DHCP request information */

typedef struct {
	UINT8 opt;
	UINT8 len;
	UINT8 type;
} t_DHCP_MSGTYPE;


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define DHCP_SRVID	54	/* server ID */

typedef struct {
	UINT8 opt;
	UINT8 len;
	IP4 ip;
} t_DHCP_SRVID;


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define DHCP_PREQLST	55	/* parameter request list */


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define DHCP_MESSAGE	56	/* message */


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define DHCP_MAXMSG	57	/* maximum message size */

typedef struct {
	UINT8 opt;
	UINT8 len;
	UINT16 size;
} t_DHCP_MAXMSG;


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define DHCP_CLASS	60	/* class identifier */

typedef struct {
	UINT8 opt;
	UINT8 len;
	UINT8 buf[1];
} t_DHCP_CLASS;


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define DHCP_CLIENTID	61	/* client identifier */

typedef struct {
	UINT8 opt;
	UINT8 len;
	UINT8 buf[1];
} t_DHCP_CLIENTID;


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define DHCP_BOOTFILE	67	/* bootfile overload */

typedef struct {
	UINT8 opt;
	UINT8 len;
	UINT8 buf[1];
} t_DHCP_BOOTFILE;


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define DHCP_WWWSRV	72	/* web server address */


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define DHCP_SYSARCH	93	/* system architecture */
	/* numbers assigned for system architecture types */
	#define DHCP_STD_X86		0x00
	#define DHCP_PC98_X86		0x01

typedef struct {
	UINT8 opt;
	UINT8 len;
	UINT16 type;
} t_DHCP_SYSARCH;


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define DHCP_NICIF	94	/* NIC Interface specifier */
	/* numbers assigned for NIC Interface types */
	#define DHCP_UNDI		0x01  /* 2 bytes of data */
	#define DHCP_PCI		0x02  /* 12 bytes of data */
	#define DHCP_PNP		0x03  /* 7 bytes of data */

typedef struct {
	UINT8 opt;
	UINT8 len;
	UINT8 type;
	UINT8 major;
	UINT8 minor;
} t_DHCP_NICIF;


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define DHCP_PLATFORMID	97
	#define DHCP_PLATFORMID_GUID	0	/* */

typedef struct {
	UINT8 opt;
	UINT8 len;
	UINT8 type;
	UINT8 buf[1];
} t_DHCP_PLATFORMID;


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define	DHCP_END	255	/* option list terminator */


#endif /* _DHCP_H */


/* EOF - $Workfile: dhcp.h $ */
