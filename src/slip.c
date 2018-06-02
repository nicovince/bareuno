#ifndef F_CPU
#include <stdio.h>
#endif
#include "slip.h"
#include "gpio.h"

#define SLIP_END 0xC0
#define SLIP_ESC 0xDB
#define SLIP_ESC_END 0xDC
#define SLIP_ESC_ESC 0xDD

void init_slip_decoder(slip_decoder_t *slip_handle, uint8_t *buf, uint8_t buf_size)
{
    slip_handle->state = SLIP_WAIT_END;
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

    /* Check if buffer is full */
    if (slip_handle->idx == slip_handle->buf_size)
    {
        /* Flush everything, once message is complete we will catch a CRC error */
        slip_handle->idx = 0;
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
