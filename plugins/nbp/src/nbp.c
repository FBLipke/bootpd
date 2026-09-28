/**
 * FBLipke PXE NBP - C mit ASM Startup
 * PXE Spec 2.1 Compliant
 * Mit TFTP Download!
 */

#include "stdlib/include/stdlib.h"
#include "stdlib/include/pxe.h"
#include "stdlib/include/dhcp.h"

/* Extern ASM Funktionen (mit underscore!) */
extern void _print_char(char c);
extern void _print_str(const char *s);
extern uint16_t _pxe_call(uint16_t func, uint16_t bx, uint16_t cx, uint16_t dx, uint16_t di, uint16_t si);
extern void _boot_jump(void);

/* Globals - using pxe_boot_info_t from pxe.h */
static pxe_boot_info_t g_boot_info;
static uint8_t g_wds_message[128];
static uint8_t g_packet_buf[1024];

/* PXE Detection - using PXE signatures from pxe.h */
static int detect_pxe(void) {
    _print_str("PXE Detection... ");
    uint32_t sig = *(uint32_t*)0xFF0E0000;
    if (sig == SIGNATURE_NBP) {
        _print_str("!PXE OK");
        _print_newline();
        return 1;
    }
    uint16_t *ptr = (uint16_t*)0xFF0E0018;
    if (*ptr == SIGNATURE_PXENV) {
        _print_str("PXENV+ OK");
        _print_newline();
        return 1;
    }
    _print_str("No PXE!");
    _print_newline();
    return 0;
}

/* Cached Packet holen */
static int get_cached_packet(void) {
    _print_str("Get Cached Info... ");
    uint16_t seg = (uint16_t)(uintptr_t)g_packet_buf;
    uint16_t status = _pxe_call(PXENV_GET_CACHED_INFO, 0x0001, 
                                sizeof(g_packet_buf), seg, 0, 0);
    if ((status & 0xFF) == 0) {
        _print_str("OK");
        _print_newline();
        return 0;
    }
    _print_str("Fail");
    _print_newline();
    return -1;
}

/* TFTP Open - using tftp_open_t from tftp.h */
static int tftp_open(uint32_t server_ip, const char *filename) {
    tftp_open_t open;
    int i;
    
    _print_str("TFTP: Opening ");
    _print_str(filename);
    _print_str(" @ ");
    print_ip(server_ip);
    _print_newline();
    
    /* Clear structure - using memset from stdlib */
    memset(&open, 0, sizeof(tftp_open_t));
    
    open.ServerIP = server_ip;
    open.Socket = 0;
    
    i = 0;
    while (filename[i] && i < 255) {
        open.Filename[i] = filename[i];
        i++;
    }
    open.Filename[i] = 0;
    
    open.Mode[0] = 'o';
    open.Mode[1] = 'c';
    open.Mode[2] = 't';
    open.Mode[3] = 'e';
    open.Mode[4] = 't';
    open.Mode[5] = 0;
    
    uint16_t seg = ((uint32_t)&open >> 16) & 0xFFFF;
    uint16_t off = (uint32_t)&open & 0xFFFF;
    
    _pxe_call(PXENV_UNDI_TFTP_OPEN, 0, 0, 0, seg, off);
    
    if (open.Status != 0) {
        _print_str("TFTP: Open failed (");
        print_hex16(open.Status);
        _print_str(")");
        _print_newline();
        return -1;
    }
    
    _print_str("TFTP: Socket=");
    print_hex16(open.Socket);
    _print_newline();
    
    return open.Socket;
}

/* TFTP Read - using tftp_read_t from tftp.h */
static int tftp_read(int socket, uint8_t *buffer, uint16_t maxlen) {
    tftp_read_t *read = (tftp_read_t*)buffer;
    
    read->BufferLen = maxlen - sizeof(tftp_read_t);
    
    uint16_t seg = ((uint32_t)read >> 16) & 0xFFFF;
    uint16_t off = (uint32_t)read & 0xFFFF;
    
    _pxe_call(PXENV_UNDI_TFTP_READ, socket, 0, 0, seg, off);
    
    if (read->Status != 0) {
        return -1;
    }
    
    return read->PacketLen;
}

/* TFTP Close */
static void tftp_close(int socket) {
    struct {
        uint16_t Status;
        uint16_t Socket;
    } __attribute__((packed)) close_pxe;
    
    close_pxe.Socket = socket;
    
    uint16_t seg = ((uint32_t)&close_pxe >> 16) & 0xFFFF;
    uint16_t off = (uint32_t)&close_pxe & 0xFFFF;
    
    _pxe_call(PXENV_UNDI_TFTP_CLOSE, 0, 0, 0, seg, off);
}

/* TFTP Download and Boot */
static void tftp_boot(uint32_t server_ip, const char *filename) {
    uint8_t *load_addr = (uint8_t*)(BOOT_LOAD_SEG * 16);
    int socket;
    int total = 0;
    int bytes;
    
    socket = tftp_open(server_ip, filename);
    if (socket < 0) {
        _print_str("TFTP: Failed to open!");
        _print_newline();
        return;
    }
    
    _print_str("TFTP: Downloading to 0x");
    print_hex16(BOOT_LOAD_SEG);
    _print_str("000...");
    _print_newline();
    
    while (total < 1024 * 1024) {  /* Max 1MB */
        bytes = tftp_read(socket, load_addr + total, 1400);
        if (bytes < 0) {
            _print_str("TFTP: Read error!");
            _print_newline();
            tftp_close(socket);
            return;
        }
        
        /* Adjust for tftp_read_t header */
        bytes -= sizeof(tftp_read_t);
        if (bytes <= 0) break;
        
        total += bytes;
        
        /* Progress */
        if ((total % 4096) == 0) {
            _print_str("TFTP: ");
            print_dec32(total);
            _print_str(" bytes...");
            _print_newline();
        }
        
        /* Last packet? */
        if (bytes < 512) break;
    }
    
    tftp_close(socket);
    
    _print_str("TFTP: Downloaded ");
    print_dec32(total);
    _print_str(" bytes");
    _print_newline();
    
    /* Jump to loaded code */
    _print_str("Booting @ 0x");
    print_hex16(BOOT_LOAD_SEG);
    _print_str("0000...");
    _print_newline();
    
    /* Far jump to loaded code - use function pointer */
    ((void (*)(void))(BOOT_LOAD_SEG * 16))();
}

/* Option Parser */
static uint8_t* find_option(uint8_t *opts, int len, uint8_t code) {
    int i = 0;
    while (i < len) {
        uint8_t opt = opts[i++];
        if (opt == DHCP_OPT_END || opt == DHCP_OPT_PAD) {
            if (opt == DHCP_OPT_END) break;
            continue;
        }
        uint8_t opt_len = opts[i++];
        if (opt == code) return &opts[i];
        i += opt_len;
    }
    return NULL;
}

static void parse_vendor_options(uint8_t *data, int len) {
    int i = 0;
    while (i < len) {
        uint8_t opt = data[i++];
        if (opt == 0xFF || opt == 0x00) break;
        uint8_t opt_len = data[i++];
        switch (opt) {
            case RBCP_BOOT_SERVER:
                _print_str("  BootServer: ");
                if (opt_len >= 4) {
                    uint32_t ip = *(uint32_t*)&data[i];
                    print_ip(ip);
                    g_boot_info.boot_server_ip = ip;
                }
                _print_newline();
                break;
            case RBCP_BOOT_ITEM:
                _print_str("  BootItem: ");
                if (opt_len >= 4) {
                    uint16_t type = *(uint16_t*)&data[i];
                    uint16_t layer = *(uint16_t*)&data[i + 2];
                    print_hex16(type);
                    _print_char('/');
                    print_hex16(layer);
                    g_boot_info.boot_item_type = type;
                    g_boot_info.boot_item_layer = layer;
                }
                _print_newline();
                break;
            case RBCP_CREDENTIALS:
                _print_str("  Credentials: 0x");
                if (opt_len >= 4) {
                    uint32_t creds = *(uint32_t*)&data[i];
                    print_hex32(creds);
                    g_boot_info.cred_types = creds;
                }
                _print_newline();
                break;
            case WDS_NEXT_ACTION:
                _print_str("  WDS Action: 0x");
                if (opt_len >= 1) {
                    uint8_t action = data[i];
                    print_hex8(action);
                    g_boot_info.wds_next_action = action;
                }
                _print_newline();
                break;
            case WDS_REQUEST_ID:
                _print_str("  WDS RequestID: 0x");
                if (opt_len >= 4) {
                    uint32_t rid = *(uint32_t*)&data[i];
                    print_hex32(rid);
                    g_boot_info.wds_request_id = rid;
                }
                _print_newline();
                break;
            case WDS_MESSAGE:
                _print_str("  WDS Message: ");
                if (opt_len > 0 && opt_len < 128) {
                    unsigned int j;
                    for (j = 0; j < opt_len && j < 127; j++)
                        g_wds_message[j] = data[i + j];
                    g_wds_message[j] = 0;
                    _print_str((char*)g_wds_message);
                }
                _print_newline();
                break;
            default:
                _print_str("  Unknown Vendor Opt: 0x");
                print_hex8(opt);
                _print_newline();
                break;
        }
        i += opt_len;
    }
}

static void parse_dhcp_options(void) {
    uint8_t *opt;
    uint8_t len;
    
    _print_str("Parse DHCP Options...");
    _print_newline();
    
    /* Option 60 - Vendor Class Identifier (VCI) */
    opt = find_option(g_packet_buf, sizeof(g_packet_buf), DHCP_OPT_VCI);
    if (opt) {
        len = opt[-1];
        _print_str("  Option 60 (VCI): ");
        if (len > 0 && len < 128) {
            unsigned int j;
            for (j = 0; j < len; j++)
                g_boot_info.vci[j] = opt[j];
            g_boot_info.vci[len] = 0;
            _print_str((char*)g_boot_info.vci);
        }
        _print_newline();
    }
    
    /* Option 54 - Server Identifier */
    opt = find_option(g_packet_buf, sizeof(g_packet_buf), DHCP_OPT_SERVER_IDENTIFIER);
    if (opt) {
        _print_str("  Option 54 (Server): ");
        if (opt[-1] >= 4) {
            uint32_t sip = *(uint32_t*)opt;
            print_ip(sip);
        }
        _print_newline();
    }
    
    /* Option 67 - Bootfile Name */
    opt = find_option(g_packet_buf, sizeof(g_packet_buf), DHCP_OPT_BOOTFILE);
    if (opt) {
        len = opt[-1];
        _print_str("  Option 67 (Bootfile): ");
        if (len > 0 && len < 128) {
            unsigned int j;
            for (j = 0; j < len; j++)
                g_boot_info.bootfile[j] = opt[j];
            g_boot_info.bootfile[len] = 0;
            _print_str((char*)g_boot_info.bootfile);
        }
        _print_newline();
    }
    
    /* Option 17 - Root Path */
    opt = find_option(g_packet_buf, sizeof(g_packet_buf), DHCP_OPT_ROOT_PATH);
    if (opt) {
        len = opt[-1];
        _print_str("  Option 17 (Root Path): ");
        if (len > 0 && len < 256) {
            unsigned int j;
            for (j = 0; j < len && j < 255; j++)
                g_boot_info.root_path[j] = opt[j];
            g_boot_info.root_path[len < 255 ? len : 255] = 0;
            _print_str((char*)g_boot_info.root_path);
        }
        _print_newline();
    }
    
    /* Option 43 - Vendor Specific */
    opt = find_option(g_packet_buf, sizeof(g_packet_buf), DHCP_OPT_VENDOROPTS);
    if (opt) {
        len = *opt;
        _print_str("  Option 43 (Vendor), len=");
        print_hex8(len);
        _print_newline();
        parse_vendor_options(opt + 1, len);
    }
}

/* Main */
void main(void) {
    memset(&g_boot_info, 0, sizeof(pxe_boot_info_t));
    g_boot_info.signature = PXE_BOOT_INFO_SIGNATURE;
    g_boot_info.length = sizeof(pxe_boot_info_t);
    
    _print_newline();
    _print_str("========================================");
    _print_newline();
    _print_str("FBLipke PXE NBP v14 (stdlib)");
    _print_newline();
    _print_str("========================================");
    _print_newline();
    
    if (!detect_pxe()) return;
    if (get_cached_packet() != 0) return;
    
    parse_dhcp_options();
    
    _print_newline();
    _print_str("Action: 0x");
    print_hex8(g_boot_info.wds_next_action);
    _print_newline();
    
    /* Debug: Zeige VCI */
    if (g_boot_info.vci[0]) {
        _print_str("VCI: ");
        _print_str((char*)g_boot_info.vci);
        _print_newline();
    }
    
    /* Use bootfile from DHCP or default */
    if (g_boot_info.bootfile[0] == 0) {
        _print_str("No bootfile in DHCP options!");
        _print_newline();
        _print_str("Using default: boot.ipxe");
        g_boot_info.bootfile[0] = 'b';
        g_boot_info.bootfile[1] = 'o';
        g_boot_info.bootfile[2] = 'o';
        g_boot_info.bootfile[3] = 't';
        g_boot_info.bootfile[4] = '.';
        g_boot_info.bootfile[5] = 'i';
        g_boot_info.bootfile[6] = 'p';
        g_boot_info.bootfile[7] = 'x';
        g_boot_info.bootfile[8] = 'e';
        g_boot_info.bootfile[9] = 0;
    }
    
    /* Use server IP from Option 43 or default */
    if (g_boot_info.boot_server_ip == 0) {
        /* Try to get from Option 54 (Server Identifier) */
        uint8_t *opt = find_option(g_packet_buf, sizeof(g_packet_buf), DHCP_OPT_SERVER_IDENTIFIER);
        if (opt && opt[-1] >= 4) {
            g_boot_info.boot_server_ip = *(uint32_t*)opt;
        } else {
            g_boot_info.boot_server_ip = 0xC0A80101;  /* Fallback: 192.168.1.1 */
        }
    }
    
    _print_str("Boot Server: ");
    print_ip(g_boot_info.boot_server_ip);
    _print_newline();
    
    _print_str("Boot File: ");
    _print_str((char*)g_boot_info.bootfile);
    _print_newline();
    
    switch (g_boot_info.wds_next_action) {
        case WDS_APPROVAL:
            _print_str("=== APPROVAL - TFTP BOOT ===");
            _print_newline();
            tftp_boot(g_boot_info.boot_server_ip, (char*)g_boot_info.bootfile);
            break;
        case WDS_REFERRAL:
            _print_str("REFERRAL - Would boot from: ");
            print_ip(g_boot_info.boot_server_ip);
            _print_newline();
            _print_str("Boot File: ");
            _print_str((char*)g_boot_info.bootfile);
            _print_newline();
            break;
        case WDS_ABORT:
            _print_str("ABORTED by WDS!");
            _print_newline();
            break;
        default:
            /* Default: try to boot anyway */
            _print_str("=== TFTP BOOT (default) ===");
            _print_newline();
            tftp_boot(g_boot_info.boot_server_ip, (char*)g_boot_info.bootfile);
            break;
    }
    
    _print_newline();
    _print_str("Halted.");
    _print_newline();
    for (;;) {
#ifdef _MSC_VER
        __asm { hlt }
#else
        __asm__ __volatile__("hlt");
#endif
    }
}
