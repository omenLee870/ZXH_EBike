#ifndef _RING_BUFFER_H
#define _RING_BUFFER_H

#include <stdint.h>

#define  ERROR_NO                    0x00
#define  ERROR_RING_BUFFE_OVERFLOW   0x01
#define  ERROR_RING_BUFFER_LEN       0x02

typedef struct{
    volatile uint32_t read_index;
    volatile uint32_t write_index;
    uint32_t size;
    uint8_t full;
    uint8_t *stroage;
}ring_buffer_t;


void user_ring_buffer_init(ring_buffer_t * ring_buffer,uint8_t *stroge,uint32_t stroge_size);
uint8_t user_ring_buffer_empty(ring_buffer_t * ring_buffer);
uint32_t user_ring_buffer_bytes_avaiable(ring_buffer_t * ring_buffer);
int user_ring_buffer_write(ring_buffer_t * ring_buffer,uint8_t *data,uint32_t length);
int user_ring_buffer_read(ring_buffer_t * ring_buffer,uint8_t *data,uint32_t length);
void user_ring_buffer_clear(ring_buffer_t * ring_buffer);
#endif
