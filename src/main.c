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

    //uint8_t buf[] = {'0', '\r', '\n'};
    while(1)
    {
        uint8_t c;
        size_t sz = usart_read(&c, 1);
        if (sz == 0)
            continue;
        if (c == 'a') {
            sbi(BOARD_PIN13_TOGGLE_REG, BOARD_PIN13_TOGGLE_BIT);
        }
        usart_write(&c, 1);
    }
    return 0;
}
