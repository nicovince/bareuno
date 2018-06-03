#ifndef __SLIP_PAYLOAD_H__
#define __SLIP_PAYLOAD_H__

#include <stdint.h>

#define MAX_SLIP_PAYLOAD 255
#define PID_OFFSET 0
#define SEQ_OFFSET 1
#define LEN_OFFSET 2
#define DATA_OFFSET 3
#define CRC_SIZE 2
#pragma pack(push, 1)
typedef struct {
    uint8_t pid; // Primitive id
    uint8_t seq; // sequence number
    uint8_t len; // data length
    uint8_t data[MAX_SLIP_PAYLOAD-DATA_OFFSET-CRC_SIZE];
    uint16_t crc; // CRC16-CCITT
} slip_payload_t;
#pragma pack(pop)
typedef void (*slip_callback_t)(slip_payload_t *msg);

uint16_t compute_crc(slip_payload_t * slip_payload);
int8_t unpack_slip_payload(uint8_t *raw_slip_payload, slip_payload_t *slip_payload);
#endif // __SLIP_PAYLOAD_H__
