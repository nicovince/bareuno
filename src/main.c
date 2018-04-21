#include <avr/io.h>
#include "board.h"

int main(void)
{
    BOARD_PIN13_DIR_REG |= _BV(BOARD_PIN13_DIR_BIT);
    BOARD_PIN13_PORT |= _BV(BOARD_PIN13_PIN);
    while(1)
    {
        asm("nop");
    }
    return 0;
}
