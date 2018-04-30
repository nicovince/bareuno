#define MAX_BOARD_OUTPUT_PINS 13
#include <avr/io.h>
#include "gpio.h"
#include "board.h"

uint8_t board_pin_to_bit_pos(uint8_t board_pin)
{
    if (board_pin < 8) {
        return board_pin;
    } else if (board_pin <= MAX_BOARD_OUTPUT_PINS) {
        return (board_pin - 8);
    }
    return UINT8_MAX;
}

uint8_t board_pin_to_port_pin(uint8_t board_pin)
{
    if (board_pin < 8) {
        return (1 << board_pin);
    } else if (board_pin <= MAX_BOARD_OUTPUT_PINS) {
        return (1 << (board_pin - 8));
    }
    return UINT8_MAX;
}

uint8_t board_pin_set(uint8_t board_pin)
{
    uint8_t bit = board_pin_to_port_pin(board_pin);
    if (bit != UINT8_MAX)
    {
        if (board_pin < 8) {
            (_SFR_BYTE(PORTD)) |= bit;
        } else if (board_pin <= MAX_BOARD_OUTPUT_PINS) {
            (_SFR_BYTE(PORTB)) |= bit;
        }
        return 0;
    }
    return 1;
}
uint8_t board_pin_clear(uint8_t board_pin)
{
    uint8_t bit = board_pin_to_port_pin(board_pin);
    if (bit != UINT8_MAX)
    {
        if (board_pin < 8) {
            (_SFR_BYTE(PORTD)) &= ~bit;
        } else if (board_pin <= MAX_BOARD_OUTPUT_PINS) {
            (_SFR_BYTE(PORTB)) &= ~bit;
        }
        return 0;
    }
    return 1;
}

uint8_t board_pin_toggle(uint8_t board_pin)
{
    uint8_t bit = board_pin_to_port_pin(board_pin);
    if (bit != UINT8_MAX)
    {
        if (board_pin < 8) {
            (_SFR_BYTE(PIND)) |= bit;
        } else if (board_pin <= MAX_BOARD_OUTPUT_PINS) {
            (_SFR_BYTE(PINB)) |= bit;
        }
        return 0;
    }
    return 1;
}

uint8_t board_pin_set_output(uint8_t board_pin)
{
    uint8_t bit = board_pin_to_port_pin(board_pin);
    if (bit != UINT8_MAX)
    {
        if (board_pin < 8) {
            (_SFR_BYTE(DDRD)) |= bit;
        } else if (board_pin <= MAX_BOARD_OUTPUT_PINS) {
            (_SFR_BYTE(DDRB)) |= bit;
        }
        return 0;
    }
    return 1;
}

uint8_t board_pin_set_input(uint8_t board_pin)
{
    uint8_t bit = board_pin_to_port_pin(board_pin);
    if (bit != UINT8_MAX)
    {
        if (board_pin < 8) {
            (_SFR_BYTE(DDRD)) &= ~bit;
        } else if (board_pin <= MAX_BOARD_OUTPUT_PINS) {
            (_SFR_BYTE(DDRB)) &= ~bit;
        }
        return 0;
    }
    return 1;
}

uint8_t board_pin_get_dir(uint8_t board_pin)
{
    uint8_t bit = board_pin_to_port_pin(board_pin);
    uint8_t dir = 0;
    if (bit != UINT8_MAX)
    {
        if (board_pin < 8) {
            dir =(_SFR_BYTE(DDRD)) & bit;
        } else if (board_pin <= MAX_BOARD_OUTPUT_PINS) {
            dir = (_SFR_BYTE(DDRB)) & bit;
        }
        if (dir) {
            return 1;
        } else {
            return 0;
        }
    }
    return UINT8_MAX;
}

