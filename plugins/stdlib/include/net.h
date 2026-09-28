/**
 * Network Utilities
 * Byte order, CRC, etc.
 */

#ifndef _NET_H
#define _NET_H

#include "stdlib.h"

/* Byte order conversions */
uint16_t htons(uint16_t x);
uint16_t ntohs(uint16_t x);
uint32_t htonl(uint32_t x);
uint32_t ntohl(uint32_t x);

/* CRC32 */
uint32_t crc32(const uint8_t *data, size_t len);

/* IP address helpers */
int parse_ip(const char *str, uint32_t *out);
void format_ip(uint32_t ip, char *buf);

/* Ethernet helpers */
void format_mac(const uint8_t *mac, char *buf);

#endif /* _NET_H */

/* CRC16 */
uint16_t crc16(const uint8_t *data, size_t len);
