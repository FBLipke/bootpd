/**
 * port_io - Port I/O operations
 */

#include <stdlib.h>

extern uint8_t  _inb(uint16_t port);
extern void     _outb(uint16_t port, uint8_t val);
extern uint16_t _inw(uint16_t port);
extern void     _outw(uint16_t port, uint16_t val);

uint8_t inb(uint16_t port) { return _inb(port); }
void outb(uint16_t port, uint8_t val) { _outb(port, val); }
uint16_t inw(uint16_t port) { return _inw(port); }
void outw(uint16_t port, uint16_t val) { _outw(port, val); }
