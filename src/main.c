#include <avr/io.h>
#include "board.h"
#include "usart.h"

int main(void)
{
    BOARD_PIN13_DIR_REG |= _BV(BOARD_PIN13_DIR_BIT);
    BOARD_PIN13_PORT &= ~_BV(BOARD_PIN13_PIN);

    setup_usart(BAUD_9600);

    uint8_t c='0';
    while(1)
    {
        usart_putchar_block(c++);
        usart_putchar_block('\r');
        usart_putchar_block('\n');
        if (c == 'Z') {
            sbi(BOARD_PIN13_TOGGLE_REG, BOARD_PIN13_TOGGLE_BIT);
            c = '0';
        }
    }
    return 0;
}
