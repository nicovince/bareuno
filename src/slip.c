#ifndef F_CPU
#include <stdio.h>
#endif
#include "slip.h"

#define SLIP_END 0xC0
#define SLIP_ESC 0xDB
#define SLIP_ESC_END 0xDC
#define SLIP_ESC_ESC 0xDD

void init_slip_decoder(slip_decoder_t *slip_handle, uint8_t *buf, uint8_t buf_size)
{
    slip_handle->state = SLIP_STORE_INCOMING;
    slip_handle->idx = 0;
    slip_handle->buf_size = buf_size;
    slip_handle->buf = buf;
}

void reset_slip_decoder(slip_decoder_t *slip_handle)
{
    slip_handle->state = SLIP_WAIT_END;
    slip_handle->idx = 0;
}

int16_t slip_decode(slip_decoder_t *slip_handle, uint8_t b)
{
    if (slip_handle->state == SLIP_WAIT_END)
    {
        if (b == SLIP_END) {
            slip_handle->state = SLIP_STORE_INCOMING;
        }
    }
    else if (slip_handle->state == SLIP_ESCAPING)
    {
        slip_handle->state = SLIP_STORE_INCOMING;
        /* There are only two bytes accepted while in escape state */
        if (b == SLIP_ESC_END) {
            slip_handle->buf[slip_handle->idx++] = SLIP_END;
        } else if (b == SLIP_ESC_ESC) {
            slip_handle->buf[slip_handle->idx++] = SLIP_ESC;
        } else {
            /* Error, let the upper layer sort it out */
            slip_handle->buf[slip_handle->idx++] = b;
        }
    }
    else if (slip_handle->state == SLIP_STORE_INCOMING)
    {
        if (b == SLIP_ESC)
        {
            /* Next byte will be an escaped one */
            slip_handle->state = SLIP_ESCAPING;
        }
        else if (b == SLIP_END)
        {
            if (slip_handle->idx > 0)
            {
                /* End of the packet */
                slip_handle->state = SLIP_WAIT_END;
                return slip_handle->idx;
            } else {
                /* Start of packet */
            }
        }
        else
        {
            /* Store regular byte */
            slip_handle->buf[slip_handle->idx++] = b;
        }
    }
    return -1;
}

int16_t slip_encode(const uint8_t *src, uint8_t *dst, int16_t len)
{

    int16_t dst_idx = 0;
    dst[dst_idx++] = SLIP_END;

    for (int16_t i = 0; i < len; ++i)
    {

        if (src[i] == SLIP_END)
        {
            dst[dst_idx++] = SLIP_ESC;
            dst[dst_idx++] = SLIP_ESC_END;
        }
        else if (src[i] == SLIP_ESC)
        {
            dst[dst_idx++] = SLIP_ESC;
            dst[dst_idx++] = SLIP_ESC_ESC;
        }
        else
        {
            dst[dst_idx++] = src[i];
        }
    }
    dst[dst_idx++] = SLIP_END;
    return dst_idx;
}
#if 0
void print_slip_status(slip_decoder_t *slip_handle);
void process_slip_frame(slip_decoder_t *slip_handle, uint8_t *f, uint8_t n);
void print_slip_status(slip_decoder_t *slip_handle)
{
    printf("state: %d\n", slip_handle->state);
    printf("idx: %d\n", slip_handle->idx);
}
int main(void)
{
    slip_decoder_t slip;
    uint8_t buf[255];
    init_slip_decoder(&slip, buf, 255);

    uint8_t f1[] = {0xC0, 0, 1, 2, 3, 4, 0xC0}; // Regular packet
    uint8_t f2[] = {0xC0, 0, 1, 0xC0, 2, 3, 4}; // Noise after
    uint8_t f3[] = {0xC0, 0xAA, 0xBB, 0xC0, 0xC0, 0x3, 0x2, 0x1, 0xC0}; // Regular packet
    uint8_t f4[] = {0xC0, 0xDB, 0xDC, 0xDC, 0xDD, 0xDB, 0xDD, 0xC0}; // Frame with esc
    process_slip_frame(&slip, f1, sizeof(f1));
    process_slip_frame(&slip, f2, sizeof(f2));
    process_slip_frame(&slip, f3, sizeof(f3));
    process_slip_frame(&slip, f4, sizeof(f4));

}

void process_slip_frame(slip_decoder_t *slip_handle, uint8_t *f, uint8_t n)
{
    uint8_t buf_encoded[255];
    for (int i = 0; i < n; ++i)
    {
        int16_t s;
        s = slip_decode(slip_handle, f[i]);
        if (s >= 0) {
            if (s)
            {
                printf("decoded frame\n");
                print_slip_status(slip_handle);
                for (int j = 0; j < s; ++j)
                {
                    printf("[%d]:%02X\n", j, (unsigned int)slip_handle->buf[j]);
                }

            }
            int16_t n = slip_encode(slip_handle->buf, buf_encoded, slip_handle->idx);
            for (int i = 0; i < n; ++i)
            {
                printf("%02X\n", buf_encoded[i]);
            }

            reset_slip_decoder(slip_handle);
        }
    }
}
#endif
