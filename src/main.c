#include <avr/io.h>
#include "board.h"
#include <avr/interrupt.h>
#include "usart.h"

int main(void)
{
    sei();
    BOARD_PIN13_DIR_REG |= _BV(BOARD_PIN13_DIR_BIT);
    BOARD_PIN13_PORT &= ~_BV(BOARD_PIN13_PIN);

    setup_usart(BAUD_9600);

    uint8_t c='0';
    uint8_t buf[] = {'0', '\r', '\n'};
    while(1)
    {
        buf[0]++;
        usart_write(buf, 3);
        if (buf[0] == 'Z') {
            buf[0] = '0';
        }
    }
    return 0;
}
