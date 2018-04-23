#ifndef __FIFO_H__
#define __FIFO_H__
#include <stdint.h>

typedef struct {
    uint8_t elt_size; /* Size of an element in the fifo */
    uint16_t n; /* Number of element in the fifo */
    uint16_t wr_idx; /* Write pointer */
    uint16_t rd_idx; /* Read pointer */
    void * array;
} fifo_t;

void init_fifo(fifo_t * handle, uint8_t elt_size, uint16_t n, void * array);
uint8_t push(fifo_t * handle, const void * elt);
uint8_t pop(fifo_t *handle, void * elt);
uint8_t is_empty(fifo_t * handle);
uint8_t is_full(fifo_t * handle);
uint16_t level(fifo_t * handle);
uint16_t free_space(fifo_t * handle);
#endif // __FIFO_H__
