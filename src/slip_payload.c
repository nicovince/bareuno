#include "slip_payload.h"
#include <string.h>
#include "crc.h"

uint16_t compute_crc(slip_payload_t * slip_payload)
{
    uint16_t crc = 0xFFFF;
    uint8_t *buf = (uint8_t *)slip_payload;
    for (int i = 0; i < slip_payload->len + DATA_OFFSET; ++i)
    {
        crc = crc_ccitt_update(crc, buf[i]);
    }
    return crc;
}

int8_t unpack_slip_payload(uint8_t *raw_slip_payload, slip_payload_t *slip_payload)
{
    slip_payload->pid = raw_slip_payload[PID_OFFSET];
    slip_payload->seq = raw_slip_payload[SEQ_OFFSET];
    slip_payload->len = raw_slip_payload[LEN_OFFSET];
    memcpy(slip_payload->data, &raw_slip_payload[DATA_OFFSET], slip_payload->len);
    slip_payload->crc = *(uint16_t *)(&raw_slip_payload[DATA_OFFSET+slip_payload->len]);
    uint16_t crc = compute_crc(slip_payload);
    return (crc == slip_payload->crc);
}
