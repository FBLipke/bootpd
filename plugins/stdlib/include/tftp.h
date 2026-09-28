/**
 * TFTP Client - Trivial File Transfer Protocol
 * RFC 1350, RFC 2347 (options), RFC 2348 (blksize)
 */

#ifndef _TFTP_H
#define _TFTP_H

#include <stdlib.h>

/* TFTP Opcodes */
#define TFTP_RRQ     1  /* Read Request */
#define TFTP_WRQ     2  /* Write Request */
#define TFTP_DATA    3  /* Data */
#define TFTP_ACK     4  /* Acknowledgment */
#define TFTP_ERROR   5  /* Error */
#define TFTP_OACK    6  /* Option Acknowledgment */

/* TFTP Error Codes */
#define TFTP_EUNDEF     0  /* Not defined */
#define TFTP_ENOTFOUND  1  /* File not found */
#define TFTP_EACCESS    2  /* Access violation */
#define TFTP_ENOSPACE   3  /* Disk full */
#define TFTP_EBADOP     4  /* Illegal TFTP operation */
#define TFTP_EBADID     5  /* Unknown transfer ID */
#define TFTP_EEXISTS    6  /* File exists */
#define TFTP_ENOUSER    7  /* No such user */

/* Default Block Size */
#define TFTP_BLKSIZE    512
#define TFTP_MAX_BLKSIZE 1428  /* Max for Ethernet + IP + UDP */

/* TFTP Transfer Info */
#ifndef tftp_transfer_defined
#define tftp_transfer_defined
struct tftp_transfer {
    uint32_t server_ip;
    uint16_t server_port;
    uint16_t local_port;
    uint8_t *buffer;
    uint32_t buffer_size;
    uint32_t bytes_received;
    uint16_t block_num;
    uint8_t  retries;
    uint8_t  state;
};
#endif

/* TFTP States */
#define TFTP_STATE_IDLE      0
#define TFTP_STATE_RRQ_SENT  1
#define TFTP_STATE_DATA      2
#define TFTP_STATE_COMPLETE  3
#define TFTP_STATE_ERROR     4

/* TFTP Functions */
#ifndef tftp_read_defined
#define tftp_read_defined
int tftp_read(uint32_t server_ip, const char *filename, void *buffer, uint32_t size);
int tftp_read_with_options(uint32_t server_ip, const char *filename, void *buffer, uint32_t size, uint16_t blksize);
const char *tftp_error_str(int errcode);
#endif

/* Extended TFTP Structures for PXE */
#ifndef tftp_open_t_defined
#define tftp_open_t_defined
typedef struct {
    uint16_t Status;
    uint32_t ServerIP;
    uint32_t GatewayIP;
    uint8_t  MCastAddr[16];
    uint8_t  ARPServerIP[4];
    uint8_t  SubnetMask[4];
    uint8_t  DNS[4];
    uint8_t  DNS2[4];
    uint8_t  Lease[4];
    uint8_t  LeaseLen;
    uint8_t  VendorClass[64];
    uint8_t  VendorClassLen;
    uint8_t  ClientUUID[16];
    uint16_t Socket;
    uint8_t  Filename[256];
    uint8_t  Mode[32];
} tftp_open_t;
#endif

#ifndef tftp_read_t_defined
#define tftp_read_t_defined
typedef struct {
    uint16_t Status;
    uint16_t PacketLen;
    uint16_t BufferLen;
    uint16_t Buffer[1];
} tftp_read_t;
#endif

/* PXE-specific TFTP macros */
#define TFTP_PXE_OPEN(server_ip, filename, open_struct) \
    tftp_pxe_open((server_ip), (filename), (open_struct))
#define TFTP_PXE_READ(socket, buffer, maxlen, read_struct) \
    tftp_pxe_read((socket), (buffer), (maxlen), (read_struct))
#define TFTP_PXE_CLOSE(socket) tftp_pxe_close(socket)

/* PXE TFTP functions */
int tftp_pxe_open(uint32_t server_ip, const char *filename, tftp_open_t *open_struct);
int tftp_pxe_read(int socket, void *buffer, uint16_t maxlen, tftp_read_t *read_struct);
void tftp_pxe_close(int socket);

#endif /* _TFTP_H */
