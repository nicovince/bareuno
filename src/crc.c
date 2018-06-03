#include "crc.h"
uint16_t crc_ccitt_update(uint16_t crc, uint8_t x)
{
    uint16_t crc_new = (unsigned char)(crc >> 8) | (crc << 8);
    crc_new ^= x;
    crc_new ^= (unsigned char)(crc_new & 0xff) >> 4;
    crc_new ^= crc_new << 12;
    crc_new ^= (crc_new & 0xff) << 5;
    crc = crc_new;
    return crc;
}
