#ifndef __GPIO_H__
#define __GPIO_H__
#include <stdint.h>
#define GREEN_LED 5
#define YELLOW_LED 7

uint8_t board_pin_to_port(uint8_t board_pin);
uint8_t board_pin_to_port_pin(uint8_t board_pin);
uint8_t board_pin_to_bit_pos(uint8_t board_pin);
uint8_t board_pin_set(uint8_t board_pin);
uint8_t board_pin_clear(uint8_t board_pin);
uint8_t board_pin_toggle(uint8_t board_pin);
uint8_t board_pin_set_output(uint8_t board_pin);
uint8_t board_pin_set_input(uint8_t board_pin);

#endif // __GPIO_H__
