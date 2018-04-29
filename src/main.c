#include <avr/io.h>
#include "board.h"
#include <avr/interrupt.h>
#include "usart.h"
#include "slip.h"
#include "slip_payload.h"
slip_decoder_t slip;
uint8_t slip_buffer[128];
uint8_t slip_encoded[256];
uint8_t raw_slip_payload[MAX_SLIP_PAYLOAD];

int main(void)
{
    sei();
    BOARD_PIN13_DIR_REG |= _BV(BOARD_PIN13_DIR_BIT);
    BOARD_PIN13_PORT &= ~_BV(BOARD_PIN13_PIN);
    int16_t slip_size;

    setup_usart(BAUD_9600);
    init_slip_decoder(&slip, slip_buffer, 128);

    uint8_t c;
    while(1)
    {
        size_t sz = usart_read(&c, 1);
        if (sz != 0) {
            slip_size = slip_decode(&slip, c);
            if (slip_size == 0) {
                /* Reset empty message */
                reset_slip_decoder(&slip);
            }
        };
        if ((sz != 0) && (slip_size > 0))
        {
            slip_payload_t slip_payload;
            unpack_slip_payload(slip.buf, &slip_payload);
            /* TODO Check length of payload data is lesser than slip message length */
            if (slip_payload.pid == 6)
            {
                sbi(BOARD_PIN13_TOGGLE_REG, BOARD_PIN13_TOGGLE_BIT);
            }
            reset_slip_decoder(&slip);
            uint16_t slip_payload_size = pack_slip_payload(&slip_payload, raw_slip_payload);
            uint8_t tx_slip_size = slip_encode(raw_slip_payload, slip_encoded, slip_payload_size);
            usart_write(slip_encoded, tx_slip_size);

        }
#if 0
        if (sz == 0)
            continue;
        if (c == 'a') {
            sbi(BOARD_PIN13_TOGGLE_REG, BOARD_PIN13_TOGGLE_BIT);
        }
#endif
    }
    return 0;
}
