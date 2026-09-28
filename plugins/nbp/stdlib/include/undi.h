/**
 * UNDI - Universal Network Driver Interface
 * UEFI Specification 2.11
 * Appendix E: UNDI C Definitions
 */

#ifndef _UNDI_H
#define _UNDI_H

#include <stdlib.h>

/* UNDI Protocol */
#define UNDI_MEDIA_UNSUPPORTED       0x0001
#define UNDI_MEDIA_CABLE_PROBLEM     0x0002
#define UNDI_MEDIA_CONNECTED         0x0004
#define UNDI_MEDIA_DISCONNECTED      0x0008

/* UNDI IOPorts */
#define UNDI_IO_MEDIA_STATE          0x00
#define UNDI_IO_PARAMS               0x01
#define UNDI_IO_RING_INFO            0x02
#define UNDI_IO_ARRAY_PORT           0x03
#define UNDI_IO_STATION_ADDRESS      0x04
#define UNDI_IO_STATISTICS           0x05
#define UNDI_IO_MULTICAST_BITS       0x06
#define UNDI_IO_MAC_ADDR             0x07

/* PXENV_UNDI_GET_INFORMATION */
typedef struct {
    uint16_t Signature;           // 'UNDI'
    uint8_t  Revision;
    uint8_t  IFtype;             // 0=PXE, 1=UNDI
    uint8_t  MajorVer;
    uint8_t  MinorVer;
    uint16_t Mode;               // 0=RealMode, 1=SoftCap, 2=Cap
    uint16_t IFnum;              // Interface number
    uint16_t #VendorDrvPad际;     // POUND# (should be a different field)
    uint8_t  BusType[80];        // Bus type string
} __attribute__((packed)) PXENV_UNDI_GET_INFORMATION_t;

/* PXENV_UNDI_OPEN */
typedef struct {
    uint16_t OpenFlag;
    uint16_t Filter;
    uint16_t MCastAddrCount;
    uint8_t  RcvBuffs;
    uint8_t  XmitBuffs;
    uint16_t Reserved;
} __attribute__((packed)) PXENV_UNDI_OPEN_t;

/* PXENV_UNDI_CLOSE */
typedef struct {
    uint16_t Reserved;
} __attribute__((packed)) PXENV_UNDI_CLOSE_t;

/* PXENV_UNDI_TRANSMIT */
typedef struct {
    uint8_t   Protocol;
    uint16_t  DestAddrLen;
    uint8_t  *DestAddr;
    uint16_t  ProtSpecificLen;
    void     *ProtSpecific;
    uint16_t  FrameLen;
    uint16_t  Reserved;
} __attribute__((packed)) PXENV_UNDI_TRANSMIT_t;

/* PXENV_UNDI_RECEIVE */
typedef struct {
    uint16_t BufferLen;
    uint8_t *Buffer;
    uint16_t ProtSpecificLen;
    void    *ProtSpecific;
    uint16_t Reserved;
} __attribute__((packed)) PXENV_UNDI_RECEIVE_t;

/* UNDI Device Info */
typedef struct {
    uint16_t DeviceHandle;        // PCI handle
    uint16_t BlockHandle;        // Block handle  
    uint8_t  IFnum;             // Interface number
    uint8_t  Duplex;            // 0=half, 1=full
    uint8_t  Speed;             // 10/100/1000 Mbps
    uint16_t MaxMcastFilter;
    uint16_t MCastFilterCount;
    uint8_t  MCastFilter[256];  // Multicast filter
    uint8_t  CurrentMcastFilter;
    uint16_t SupportedTypes;     //Supported types
    uint8_t  FrameLen[2];       // Frame lengths
    uint16_t HardwareAddrLen;
    uint8_t  HardwareAddr[32];  // MAC address
} __attribute__((packed)) UNDI_DEVICE_INFO;

/* UNDI Configuration */
typedef struct {
    uint16_t Bus;
    uint16_t Device;
    uint16_t Function;
    uint16_t VendorID;
    uint16_t DeviceID;
    uint16_t Class;
    uint8_t  Revision;
    uint8_t  BusType;
    uint8_t  MemorySpace:1;
    uint8_t  Dma:1;
    uint8_t  BusMaster:1;
    uint8_t  IoSpace:1;
} __attribute__((packed)) UNDI_CONFIG_INFO;

/* UNDI API Functions (function pointers) */
typedef struct {
    void *reserved[4];
} __attribute__((packed)) UNDI_PROT_INFO;

typedef struct {
    uint16_t (*undi_start)(uint16_t DeviceHandle);
    uint16_t (*undi_stop)(uint16_t DeviceHandle);
    uint16_t (*undi_get_state)(uint16_t DeviceHandle, uint16_t *State);
    uint16_t (*undi_get_information)(uint16_t DeviceHandle, PXENV_UNDI_GET_INFORMATION_t *Info);
    uint16_t (*undi_open)(uint16_t DeviceHandle, PXENV_UNDI_OPEN_t *Open);
    uint16_t (*undi_close)(uint16_t DeviceHandle, PXENV_UNDI_CLOSE_t *Close);
    uint16_t (*undi_initialize)(uint16_t DeviceHandle, uint16_t *Flag, uint16_t *MTU);
    uint16_t (*undi_reset)(uint16_t DeviceHandle);
    uint16_t (*undi_shutdown)(uint16_t DeviceHandle);
    uint16_t (*undi_receive)(uint16_t DeviceHandle, PXENV_UNDI_RECEIVE_t *Receive);
    uint16_t (*undi_transmit)(uint16_t DeviceHandle, PXENV_UNDI_TRANSMIT_t *Transmit);
    uint16_t (*undi_set_station_addr)(uint16_t DeviceHandle, uint8_t *MCastAddr);
    uint16_t (*undi_set_parameters)(uint16_t DeviceHandle);
    uint16_t (*undi_set_packet_filter)(uint16_t DeviceHandle, uint16_t Filter);
    uint16_t (*undi_get_statistics)(uint16_t DeviceHandle);
    uint16_t (*undi_read_mac_addr)(uint16_t DeviceHandle);
    uint16_t (*undi_read_config_data)(uint16_t DeviceHandle);
    uint16_t (*undi_write_config_data)(uint16_t DeviceHandle);
} UNDI_CALLS_t;

/* Service Functions */
typedef struct {
    uint8_t  MACAddr[32];
    uint16_t MACAddrLen;
    uint16_t MediaHeaderLen;
    uint16_t MTU;
    uint16_t HWType;
    uint32_t Status;
} __attribute__((packed)) UNDI_GET_STATUS_t;

typedef struct {
    uint16_t Flags;
    uint16_t Length;
    uint16_t XmitReserved;
} __attribute__((packed)) UNDI_ISR_t;

/* UNDI Status Codes */
#define UNDI_SUCCESS                0x0000
#define UNDI_FAILURE                0x0001
#define UNDI_NOT_OPENED            0x0002
#define UNDI_NOT_STARTED           0x0003
#define UNDI_INVALID_PARAMETER     0x0004
#define UNDI_NO_SPACE              0x0005
#define UNDI_UNSUPPORTED           0xFFFE
#define UNDI_ERROR                 0xFFFF

/* UNDI States */
#define UNDI_STATE_CLOSED          0x0000
#define UNDI_STATE_OPENED          0x0001
#define UNDI_STATE_INITIALIZED     0x0002
#define UNDI_STATE_GET_STATE       0x0003

#endif /* _UNDI_H */
