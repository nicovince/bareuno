#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "slip.h"
#include "slip_payload.h"

void print_slip_payload(slip_payload_t * slip_payload)
{
    printf("pid: %d\n", slip_payload->pid);
    printf("seq: %d\n", slip_payload->seq);
    printf("len: %d\n", slip_payload->len);
    for (uint8_t i = 0; i < slip_payload->len; ++i)
    {
        printf("data[%d] %02X\n", i, slip_payload->data[i]);
    }
    printf("crc: %02X\n", slip_payload->crc);
}
    

bool check_unpack(slip_payload_t* ref, uint8_t *raw_slip_payload)
{
    slip_payload_t slip_unpacked;
    unpack_slip_payload(raw_slip_payload, &slip_unpacked);
    print_slip_payload(ref);
    print_slip_payload(&slip_unpacked);
    
    bool res = true;
    res &= (ref->pid == slip_unpacked.pid);
    res &= (ref->seq == slip_unpacked.seq);
    res &= (ref->len == slip_unpacked.len);
    res &= !memcmp(ref->data, slip_unpacked.data, ref->len);
    res &= (ref->crc == slip_unpacked.crc);
    return res;
}

int main(int argc, char** argv)
{
    uint8_t raw_slip_payload[MAX_SLIP_PAYLOAD];
    uint8_t slip_buffer[2*MAX_SLIP_PAYLOAD];
    char * arduino = argv[1];
    uint8_t pid = (uint8_t)atoi(argv[2]);
    slip_payload_t slip_payload;
    memset(raw_slip_payload, 0 , MAX_SLIP_PAYLOAD);
    srand(pid);
    
    slip_payload.pid = pid;
    slip_payload.seq = (uint8_t) rand();
    slip_payload.len = 7;
    
    printf("%s : %d\n", arduino, pid);
    uint16_t slip_payload_size = pack_slip_payload(&slip_payload, raw_slip_payload);
    
    bool res = check_unpack(&slip_payload, raw_slip_payload);
    printf("check_unpack: %d\n", res);

    uint8_t slip_size = slip_encode(raw_slip_payload, slip_buffer, slip_payload_size);
    

    return 0;
}
