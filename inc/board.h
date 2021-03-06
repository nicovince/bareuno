#include <avr/io.h>

#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))

#define BOARD_PIN0_MCU_PORT PORTD
#define BOARD_PIN0_MCU_PIN  PORTD0

#define BOARD_PIN1_MCU_PORT PORTD
#define BOARD_PIN1_MCU_PIN  PORTD1

// ...

// pin 13 / Led
#define BOARD_PIN13_PORT PORTB
#define BOARD_PIN13_PIN  PORTB5
#define BOARD_PIN13_DIR_REG DDRB
#define BOARD_PIN13_DIR_BIT DDB5
#define BOARD_PIN13_TOGGLE_REG PINB
#define BOARD_PIN13_TOGGLE_BIT PINB5

