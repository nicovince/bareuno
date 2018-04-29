#ifndef __GPIO_H__
#define __GPIO_H__
#include <stdint.h>

uint8_t board_pin_to_port(uint8_t board_pin);
uint8_t board_pin_to_port_pin(uint8_t board_pin);
uint8_t board_pin_set(uint8_t board_pin);
uint8_t board_pin_clear(uint8_t board_pin);

#endif // __GPIO_H__
