#include <avr/io.h>
#include <avr/interrupt.h>

#include "usart.h"
#include "fifo.h"
#include "board.h"

#define TX_FIFO_SIZE 32
#define RX_FIFO_SIZE 32
fifo_t tx_fifo;
uint8_t tx_array[TX_FIFO_SIZE];
//fifo_t rx_fifo;
//uint8_t rx_array[RX_FIFO_SIZE];

void setup_usart(baudrate_t baudrate)
{
    /* Baudrate register depends on u2x0 bit */
    uint8_t u2x0 = (UCSR0A & _BV(U2X0));
    uint16_t brr = (F_CPU/(u2x0?4:8)/baudrate -1)/2;
    UBRR0L = brr;
    UBRR0H = (brr >> 8);

    /* Enable Tx and Rx */
    UCSR0B |= (_BV(TXEN0) | _BV(RXEN0));

    /* asynchronous, 8bits, no parity, 1 stop bit */
    UCSR0C |= (USART_CHAR_SIZE8 | STOP_1BIT | NO_PARITY | ASYNC_USART);

    /* Setup fifos */
    init_fifo(&tx_fifo, sizeof(uint8_t), TX_FIFO_SIZE, tx_array);
    //init_fifo(&rx_fifo, sizeof(uint8_t), RX_FIFO_SIZE, rx_array);
}

ISR(USART_UDRE_vect)
{
    if (is_empty(&tx_fifo)) {
        /* disable data register empty interrupt */
        cbi(UCSR0B, UDRIE0);
    } else {
        uint8_t data;
        pop(&tx_fifo, &data);
        UDR0 = data;
    }
}

int usart_write(uint8_t *buf, size_t len)
{
    for (size_t i = 0; i < len; ++i)
    {
        /* Wait until there is room in tx fifo */
        while(is_full(&tx_fifo));
        push(&tx_fifo, &buf[i]);

        /* Enable data register empty interrupt */
        sbi(UCSR0B, UDRIE0);

        if (UCSR0A & _BV(UDRE0)) {
            uint8_t data;
            pop(&tx_fifo, &data);
            UDR0 = data;
        }
    }
    return len;
}

int usart_putchar(char c)
{
    /* Check transmit buffer empty */
    if (UCSR0A & (1 << UDRE0))
    {
        UDR0 = c;
        return (int) c;
    }
    return -1;
}

void usart_putchar_block(unsigned char c)
{
    /* Wait until transmit buffer available */
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}
