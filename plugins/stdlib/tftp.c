/**
 * TFTP Client - Using PXE ROM built-in TFTP
 * RFC 1350, RFC 2347, RFC 2348
 */

#include <stdlib.h>
#include <dhcp.h>
#include <net.h>
#include <tftp.h>

/* PXE API function numbers */
#define PXENV_UNDI_TFTP_OPEN           0x0020
#define PXENV_UNDI_TFTP_READ_FILE      0x0021
#define PXENV_UNDI_TFTP_CLOSE          0x0022

/* PXE TFTP Close structure (local, not in tftp.h) */
typedef struct {
    uint16_t Status;
    uint16_t Socket;
} tftp_close_t;

/* External PXE call function (from startup.asm) */
extern uint16_t _pxe_call(uint16_t func, uint16_t ax, uint16_t cx, 
                          uint16_t dx, uint16_t di, uint16_t si);

/* Build RRQ packet (for manual TFTP) */
int tftp_build_rrq(uint8_t *packet, const char *filename, uint16_t blksize) {
    int i = 0;
    
    /* Opcode RRQ */
    packet[i++] = 0x00;
    packet[i++] = TFTP_RRQ;
    
    /* Filename */
    int j = 0;
    while (filename[j] && j < 256) {
        packet[i++] = filename[j++];
    }
    packet[i++] = 0;
    
    /* Mode "octet" */
    packet[i++] = 'o';
    packet[i++] = 'c';
    packet[i++] = 't';
    packet[i++] = 'e';
    packet[i++] = 't';
    packet[i++] = 0;
    
    /* Options (blksize) */
    if (blksize != TFTP_BLKSIZE) {
        /* Option: blksize */
        packet[i++] = 'b';
        packet[i++] = 'l';
        packet[i++] = 'k';
        packet[i++] = 's';
        packet[i++] = 'i';
        packet[i++] = 'z';
        packet[i++] = 'e';
        packet[i++] = 0;
        
        /* Value */
        int digits = 0;
        int v = blksize;
        char rev[8];
        while (v > 0) {
            rev[digits++] = '0' + (v % 10);
            v /= 10;
        }
        int k;
        for (k = 0; k < digits; k++) {
            packet[i++] = rev[digits - 1 - k];
        }
        packet[i++] = 0;
    }
    
    return i;
}

/* Build ACK packet */
int tftp_build_ack(uint8_t *packet, uint16_t block) {
    packet[0] = 0x00;
    packet[1] = TFTP_ACK;
    packet[2] = (block >> 8) & 0xFF;
    packet[3] = block & 0xFF;
    return 4;
}

/* Build ERROR packet */
int tftp_build_error(uint8_t *packet, uint16_t code, const char *msg) {
    int i = 0;
    packet[i++] = 0x00;
    packet[i++] = TFTP_ERROR;
    packet[i++] = (code >> 8) & 0xFF;
    packet[i++] = code & 0xFF;
    
    int j = 0;
    while (msg[j]) {
        packet[i++] = msg[j++];
    }
    packet[i++] = 0;
    
    return i;
}

/* Parse DATA packet */
int tftp_parse_data(uint8_t *packet, int len, uint16_t *block, uint8_t **data, int *datalen) {
    if (len < 4) return -1;
    if (packet[1] != TFTP_DATA) return -1;
    
    *block = ((uint16_t)packet[2] << 8) | packet[3];
    *data = &packet[4];
    *datalen = len - 4;
    
    return 0;
}

/* Get error string */
const char *tftp_error_str(int errcode) {
    switch (errcode) {
        case TFTP_EUNDEF:     return "Undefined error";
        case TFTP_ENOTFOUND:  return "File not found";
        case TFTP_EACCESS:    return "Access violation";
        case TFTP_ENOSPACE:   return "Disk full";
        case TFTP_EBADOP:     return "Illegal operation";
        case TFTP_EBADID:     return "Unknown transfer ID";
        case TFTP_EEXISTS:    return "File exists";
        case TFTP_ENOUSER:    return "No such user";
        default:               return "Unknown error";
    }
}

/* TFTP Open using PXE ROM */
int tftp_pxe_open(uint32_t server_ip, const char *filename, tftp_open_t *open_struct) {
    int i;
    
    /* Clear structure */
    for (i = 0; i < sizeof(tftp_open_t); i++) {
        ((uint8_t*)open_struct)[i] = 0;
    }
    
    /* Fill structure */
    open_struct->ServerIP = server_ip;
    open_struct->Socket = 0;  /* Auto-assign */
    
    /* Filename */
    i = 0;
    while (filename[i] && i < 255) {
        open_struct->Filename[i] = filename[i];
        i++;
    }
    open_struct->Filename[i] = 0;
    
    /* Mode */
    open_struct->Mode[0] = 'o';
    open_struct->Mode[1] = 'c';
    open_struct->Mode[2] = 't';
    open_struct->Mode[3] = 'e';
    open_struct->Mode[4] = 't';
    open_struct->Mode[5] = 0;
    
    /* Call PXE API */
    uint16_t seg = ((uint32_t)open_struct >> 16) & 0xFFFF;
    uint16_t off = (uint32_t)open_struct & 0xFFFF;
    
    _pxe_call(PXENV_UNDI_TFTP_OPEN, 0, 0, 0, seg, off);
    
    if (open_struct->Status != 0) {
        return -1;
    }
    
    return open_struct->Socket;
}

/* TFTP Read using PXE ROM */
int tftp_pxe_read(int socket, void *buffer, uint16_t maxlen, tftp_read_t *read_struct) {
    /* Buffer layout: read struct followed by data area */
    read_struct->BufferLen = maxlen - sizeof(tftp_read_t);
    
    /* Call PXE API */
    uint16_t seg = ((uint32_t)read_struct >> 16) & 0xFFFF;
    uint16_t off = (uint32_t)read_struct & 0xFFFF;
    
    /* Set socket in CX */
    _pxe_call(PXENV_UNDI_TFTP_READ_FILE, socket, 0, 0, seg, off);
    
    if (read_struct->Status != 0) {
        return -1;
    }
    
    return read_struct->PacketLen;
}

/* TFTP Close using PXE ROM */
void tftp_pxe_close(uint16_t socket) {
    tftp_close_t close;
    
    close.Socket = socket;
    
    uint16_t seg = ((uint32_t)&close >> 16) & 0xFFFF;
    uint16_t off = (uint32_t)&close & 0xFFFF;
    
    _pxe_call(PXENV_UNDI_TFTP_CLOSE, 0, 0, 0, seg, off);
}

/* TFTP Read using PXE ROM (high-level) */
int tftp_read_pxe(uint32_t server_ip, const char *filename, void *buffer, uint32_t size) {
    uint8_t *buf = (uint8_t*)buffer;
    int socket;
    int total = 0;
    int bytes;
    
    /* Open TFTP connection */
    socket = tftp_pxe_open(server_ip, filename);
    if (socket < 0) {
        return -1;
    }
    
    /* Read data */
    while (total < size) {
        int chunk = size - total;
        if (chunk > 1400) chunk = 1400;  /* Max packet size */
        
        bytes = tftp_pxe_read(socket, buf + total, chunk + sizeof(tftp_read_t));
        if (bytes < 0) {
            tftp_pxe_close(socket);
            return -1;
        }
        
        /* Adjust for actual data (skip tftp_read_t header) */
        bytes -= sizeof(tftp_read_t);
        if (bytes <= 0) break;
        
        total += bytes;
        
        /* Last packet? */
        if (bytes < 512) break;
    }
    
    /* Close */
    tftp_pxe_close(socket);
    
    return total;
}

/* Simple TFTP Read (default 512 byte blocks) */
int tftp_read(uint32_t server_ip, const char *filename, void *buffer, uint32_t size) {
    return tftp_read_pxe(server_ip, filename, buffer, size);
}

/* TFTP Read with options (uses PXE built-in) */
int tftp_read_with_options(uint32_t server_ip, const char *filename, void *buffer, uint32_t size, uint16_t blksize) {
    /* PXE ROM handles options automatically */
    (void)blksize;
    return tftp_read_pxe(server_ip, filename, buffer, size);
}
