#ifndef __CRC_H__
#define __CRC_H__
#include <stdint.h>
#include <string.h>

uint16_t crc_ccitt_update(uint16_t crc, uint8_t x);
unsigned short CRCCCITT(unsigned char *data, size_t length, unsigned short seed, unsigned short final);
#endif // __CRC_H__
