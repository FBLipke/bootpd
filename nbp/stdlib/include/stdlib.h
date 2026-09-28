/**
 * FBLipke Freestanding Stdlib
 * Für 16-bit Real Mode / BIOS / Bootloader
 */

#ifndef _STDLIB_H
#define _STDLIB_H

/* Basic Types */
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long      uintptr_t;
typedef unsigned long     size_t;

#define NULL ((void*)0)

/* String Functions */
void *memset(void *s, int c, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
void *memcmp(const void *s1, const void *s2, size_t n);
void *memmove(void *dest, const void *src, size_t n);
size_t strlen(const char *s);
char *strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);
int strcmp(const char *s1, const char *s2);

/* I/O Functions */
void print_char(char c);
void print_str(const char *s);
void print_newline(void);
void print_hex8(uint8_t val);
void print_hex16(uint16_t val);
void print_hex32(uint32_t val);
void print_dec16(uint16_t val);
void print_dec32(uint32_t val);
void print_ip(uint32_t ip);

/* Formatted I/O */
int printf(const char *fmt, ...);
int sprintf(char *buf, const char *fmt, ...);

/* I/O Ports */
uint8_t  inb(uint16_t port);
void     outb(uint16_t port, uint8_t val);
uint16_t inw(uint16_t port);
void     outw(uint16_t port, uint16_t val);

/* Memory */
void *malloc(size_t size);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);
size_t heap_available(void);

/* Keyboard */
int kbhit(void);
int getch(void);
int getchar(void);

/* Misc */
void delay(uint32_t ticks);
void clear(void);

/* List */
struct list_node { void *data; struct list_node *next; };
struct list { struct list_node *head; struct list_node *tail; int count; };
void list_init(struct list *l);
void list_add(struct list *l, void *data);
void *list_get(struct list *l, int index);
int list_count(struct list *l);
void list_remove(struct list *l, int index);
void list_clear(struct list *l);
void *list_iterate(struct list *l, int *state);

/* DHCP Parser */
#include <dhcp.h>
uint8_t *find_dhcp_option(uint8_t *opts, int len, uint8_t code);
int get_option_len(uint8_t *opts, int len, uint8_t code);
int parse_option_ip(uint8_t *opt, uint32_t *out);
int parse_option_str(uint8_t *opt, int max_len, char *out);
int parse_vci_arch(const char *vci);
const char *dhcp_msg_type_name(uint8_t type);

/* Network Utils */
#include <net.h>
uint16_t htons(uint16_t x);
uint16_t ntohs(uint16_t x);
uint32_t htonl(uint32_t x);
uint32_t ntohl(uint32_t x);
uint32_t crc32(const uint8_t *data, size_t len);
int parse_ip(const char *str, uint32_t *out);

#endif /* _STDLIB_H */

/* TFTP Client */
#include <tftp.h>
int tftp_read(uint32_t server_ip, const char *filename, void *buffer, uint32_t size);
int tftp_read_with_options(uint32_t server_ip, const char *filename, void *buffer, uint32_t size, uint16_t blksize);
const char *tftp_error_str(int errcode);

/* Disk/HDD Support */
#include <disk.h>
int disk_reset(uint8_t drive);
int disk_read(uint8_t drive, uint32_t lba, uint8_t count, uint8_t *buffer);
int disk_read_mbr(uint8_t drive, mbr_t *mbr);
void disk_boot_from_hdd(uint8_t drive);
void disk_boot_from_partition(uint8_t drive, int partition_num);

/* El Torito CD-ROM Boot Catalog */
#include <eltorito.h>
