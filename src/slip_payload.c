#include "slip_payload.h"
#include <string.h>
#include "crc.h"
void update_crc(slip_payload_t * slip_payload)
{
    uint16_t crc = 0xFFFF;
    uint8_t *buf = (uint8_t *)slip_payload;
    for (int i = 0; i < slip_payload->len + DATA_OFFSET; ++i)
    {
        crc = crc_ccitt_update(crc, buf[i]);
    }
    slip_payload->crc = crc;
}

int8_t unpack_slip_payload(uint8_t *raw_slip_payload, slip_payload_t *slip_payload)
{
    slip_payload->pid = raw_slip_payload[PID_OFFSET];
    slip_payload->seq = raw_slip_payload[SEQ_OFFSET];
    slip_payload->len = raw_slip_payload[LEN_OFFSET];
    memcpy(slip_payload->data, &raw_slip_payload[DATA_OFFSET], slip_payload->len);
    slip_payload->crc = *(uint16_t *)(&raw_slip_payload[DATA_OFFSET+slip_payload->len]);
    unsigned short crc = CRCCCITT(raw_slip_payload, slip_payload->len + DATA_OFFSET, 0xFFFF, 0x0);
    return (crc == slip_payload->crc);
}

uint16_t pack_slip_payload(slip_payload_t *slip_payload, uint8_t *raw_slip_payload)
{
    unsigned short crc;
    raw_slip_payload[PID_OFFSET] = slip_payload->pid;
    raw_slip_payload[SEQ_OFFSET] = slip_payload->seq;
    raw_slip_payload[LEN_OFFSET] = slip_payload->len;

    if (slip_payload->len > 0) {
        memcpy(&raw_slip_payload[DATA_OFFSET], slip_payload->data, slip_payload->len);
    }

    crc = CRCCCITT(raw_slip_payload, slip_payload->len + DATA_OFFSET, 0xFFFF, 0x0);
    *(uint16_t *)(&raw_slip_payload[DATA_OFFSET + slip_payload->len]) = crc;
    /* Update CRC of input struct as well */
    slip_payload->crc = crc;
    return DATA_OFFSET + slip_payload->len + sizeof(crc);
}
