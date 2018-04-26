#include "fifo.h"
#include <string.h>
#include <stdio.h>

typedef struct {
    uint32_t idx;
    int foo;
    char c;
    uint16_t bar;
} elt_t;

void init_fifo(fifo_t * handle, uint8_t elt_size, uint16_t n, void * array)
{
    /* Clear fifo */
    memset(handle, 0, sizeof(fifo_t));
    /* Set garbage in array */
    memset(array, 0x55, n*elt_size);

    handle->elt_size = elt_size;
    handle->n = n;
    handle->array = array;
}
uint8_t push(fifo_t * handle, const void * elt)
{
    uint16_t next = (handle->wr_idx + 1) % handle->n;
    if (next == handle->rd_idx) {
        return -1;
    }
    uint16_t byte_offset = handle->elt_size * handle->wr_idx;
    memcpy(handle->array + byte_offset, elt, handle->elt_size);
    handle->wr_idx = next;
    return 0;
}
uint8_t pop(fifo_t *handle, void * elt)
{
    if (handle->wr_idx == handle->rd_idx) {
        return -1;
    }

    uint16_t byte_offset = handle->elt_size * handle->rd_idx;
    memcpy(elt, handle->array + byte_offset, handle->elt_size);
    handle->rd_idx = (handle->rd_idx + 1) % handle->n;
    return 0;
}

uint8_t is_empty(fifo_t *handle)
{
    return (handle->rd_idx == handle->wr_idx);
}

uint8_t is_full(fifo_t *handle)
{
    return (((handle->wr_idx + 1) % handle->n) == handle->rd_idx);
}

uint16_t level(fifo_t * handle)
{
    if (handle->wr_idx >= handle-> rd_idx)
    {
        return handle->wr_idx - handle->rd_idx;
    } else {
        return handle->n - handle->rd_idx + handle->wr_idx;
    }
}

uint16_t free_space(fifo_t * handle)
{
    return handle->n - level(handle) -1;
}

#ifndef F_CPU
void print_fifo_status(fifo_t * handle)
{
    printf("elt_size: %d\n", handle->elt_size);
    printf("n: %d\n", handle->n);
    printf("wr_idx: %d\n", handle->wr_idx);
    printf("rd_idx: %d\n", handle->rd_idx);
    printf("level: %d\n", level(handle));
    printf("free_space: %d\n", free_space(handle));
}
#define FIFO_SIZE 25
#define FIFO_ELT_TYPE elt_t
int main(void)
{
    fifo_t fifo;
    FIFO_ELT_TYPE array[FIFO_SIZE];
    init_fifo(&fifo, sizeof(FIFO_ELT_TYPE), FIFO_SIZE, array);
    FIFO_ELT_TYPE elt;
    uint8_t ret;

    elt.idx = 0xCAFECAFE;
    ret = push(&fifo, &elt);
    printf("Pushing %08X -> %d\n", elt.idx, ret);
    elt.idx = 0;
    ret = pop(&fifo, &elt);
    printf("Popping %08X -> %d\n", elt.idx, ret);

    elt.idx = 0xBABEFACE;
    for (int i = 0; i<FIFO_SIZE; ++i)
    {

        ret = push(&fifo, &elt);
        printf("[F:%d;E:%d]Pushing %08X -> %d\n", is_full(&fifo), is_empty(&fifo), elt.idx, ret);
        elt.idx++;
    }
    for (int i = 0; i < FIFO_SIZE/2; ++i)
    {
        ret = pop(&fifo, &elt);
        printf("[F:%d;E:%d]popping %08X -> %d\n", is_full(&fifo), is_empty(&fifo), elt.idx, ret);

    }
    print_fifo_status(&fifo);

    elt.idx = 0;
    while(!is_full(&fifo)){
        ret = push(&fifo, &elt);
        printf("[F:%d;E:%d]Pushing %08X -> %d\n", is_full(&fifo), is_empty(&fifo), elt.idx, ret);
        elt.idx++;
    }
    print_fifo_status(&fifo);

    while (!is_empty(&fifo))
    {
        ret = pop(&fifo, &elt);
        printf("[F:%d;E:%d]popping %08X -> %d\n", is_full(&fifo), is_empty(&fifo), elt.idx, ret);

    }
    print_fifo_status(&fifo);
}
#endif
