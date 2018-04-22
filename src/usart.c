#include <avr/io.h>

#include "usart.h"

void setup_usart(baudrate_t baudrate)
{
    /* Baudrate register depends on u2x0 bit */
    uint8_t u2x0 = (UCSR0A & _BV(U2X0));
    uint16_t brr = (F_CPU/(u2x0?4:8)/baudrate -1)/2;
    //brr = 8;
    UBRR0L = brr;
    UBRR0H = (brr >> 8);

    /* Enable Tx and Rx */
    UCSR0B |= (_BV(TXEN0) | _BV(RXEN0));

    /* asynchronous, 8bits, no parity, 1 stop bit */
    UCSR0C |= (USART_CHAR_SIZE8 | STOP_1BIT | NO_PARITY | ASYNC_USART);
}

int usart_putchar(char c)
{
    if (UCSR0A & (1 << UDRE0))
    {
        UDR0 = c;
        return (int) c;
    }
    return -1;
}

void usart_putchar_block(unsigned char c)
{
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}
