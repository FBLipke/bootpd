/**
 * PXE Constants and Structures
 * PXE Spec 2.1 Compliant
 */

#ifndef _PXE_H
#define _PXE_H

#include <addresses.h>

/* PXE API Functions */
#define PXENV_GET_CACHED_INFO   0x0070
#define PXENV_UNDI_TFTP_OPEN   0x0020
#define PXENV_UNDI_TFTP_READ   0x0021
#define PXENV_UNDI_TFTP_CLOSE  0x0022
#define PXENV_UNDI_STARTUP     0x0001
#define PXENV_UNDI_SHUTDOWN    0x0002
#define PXENV_UNDI_INITIALIZE  0x0003
#define PXENV_UNDI_RESET       0x0004
#define PXENV_UNDI_MCAST_SYNC  0x0025
#define PXENV_UNDI_MCAST_GETFILTER 0x0026
#define PXENV_UNDI_MCAST_SETFILTER 0x0027

/* Signatures */
#define SIGNATURE_PXENV        0x4E50    /* "PXEN" */
#define SIGNATURE_NBP          0x21505845 /* "!PXE" */

/* Memory Segments */
#define BOOT_INFO_SEG          0x0800
#define BOOT_LOAD_SEG          0x1000

/* WDS Actions */
#define WDS_APPROVAL           1
#define WDS_REFERRAL           3
#define WDS_ABORT              5

/* PXE Boot Item Types */
#define RBCP_BOOT_SERVER       8
#define RBCP_BOOT_ITEM         71
#define RBCP_CREDENTIALS       12

/* WDS Options (inside Option 43) */
#define WDS_NEXT_ACTION        2
#define WDS_REQUEST_ID         5
#define WDS_MESSAGE            6

#pragma pack(push, 1)

/* TFTP Open structure */
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
} pxe_tftp_open_t;

/* TFTP Read structure */
typedef struct {
    uint16_t Status;
    uint16_t PacketLen;
    uint16_t BufferLen;
    uint16_t Buffer[1];
} pxe_tftp_read_t;

/* Boot Info Structure - stored at BOOT_INFO_SEG:0 */
typedef struct {
    uint16_t signature;
    uint16_t length;
    uint32_t boot_server_ip;
    uint16_t boot_item_type;
    uint16_t boot_item_layer;
    uint32_t cred_types;
    uint8_t  wds_next_action;
    uint32_t wds_request_id;
    uint8_t  vci[128];
    uint8_t  bootfile[128];
    uint8_t  root_path[256];
    uint8_t  reserved[128];
} pxe_boot_info_t;

#pragma pack(pop)

/* Boot Info signature for validation */
#define PXE_BOOT_INFO_SIGNATURE  0x4942  /* "IB" */

/* Functions */
int pxe_detect(void);
int pxe_get_cached_info(void *buffer, uint16_t size);
int pxe_tftp_open(uint32_t server_ip, const char *filename);
int pxe_tftp_read(int socket, void *buffer, uint16_t maxlen);
void pxe_tftp_close(int socket);
void pxe_boot(uint32_t server_ip, const char *filename);

#endif /* _PXE_H */
