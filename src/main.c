#include <avr/io.h>
#include "board.h"
#include <avr/interrupt.h>
#include "usart.h"
#include "slip.h"
slip_decoder_t slip;
uint8_t slip_buffer[128];
uint8_t slip_encoded[256];

int main(void)
{
    sei();
    BOARD_PIN13_DIR_REG |= _BV(BOARD_PIN13_DIR_BIT);
    BOARD_PIN13_PORT &= ~_BV(BOARD_PIN13_PIN);

    setup_usart(BAUD_9600);
    init_slip_decoder(&slip, slip_buffer, 128);

    //uint8_t buf[] = {'0', '\r', '\n'};
    uint8_t c;
    while(1)
    {
        int16_t s = slip_encode(&c, slip_encoded, 1);
        usart_write(slip_encoded, s);
        ++c;

        size_t sz = usart_read(&c, 1);
        if (sz == 0)
            continue;
        if (c == 'a') {
            sbi(BOARD_PIN13_TOGGLE_REG, BOARD_PIN13_TOGGLE_BIT);
        }
    }
    return 0;
}
