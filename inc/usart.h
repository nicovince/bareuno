#ifndef __USART_H__
#define __USART_H__
typedef enum baudrate_e {
    BAUD_9600=9600,
    BAUD_57600=57600
} baudrate_t;
        
// usart mode field for usart control and status reg 0 C (UCSR0C)
#define ASYNC_USART (0 << UMSEL00)
#define SYNC_USART (1 << UMSEL00)
#define MASTER_SPI (2 << UMSEL00)

// parity field (UCSR0C)
#define NO_PARITY (0 << UPM00)
#define EVEN_PARITY (2 << UPM00)
#define ODD_PARITY (3 << UPM00)

// stop bit field (UCSR0C)
#define STOP_1BIT (0 << USBS0)
#define STOP_2BIT (1 << USBS0)

// Char size field (UCSR0C)
#define USART_CHAR_SIZE5 (0 << UCSZ00)
#define USART_CHAR_SIZE6 (1 << UCSZ00)
#define USART_CHAR_SIZE7 (2 << UCSZ00)
#define USART_CHAR_SIZE8 (3 << UCSZ00)

void setup_usart(baudrate_t baudrate);
int usart_putchar(char c);
void usart_putchar_block(unsigned char c);

#endif // __USART_H__
