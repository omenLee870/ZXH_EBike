#include <stdint.h>
#include "user_ring_buffer.h"
#include <string.h>


#define MIN(a,b)    ((a) < (b) ? (a):(b))

void user_ring_buffer_init(ring_buffer_t * ring_buffer,uint8_t *stroge,uint32_t stroge_size)
{
    ring_buffer->stroage = stroge;
    ring_buffer->size = stroge_size;
    ring_buffer->read_index = 0;
    ring_buffer->write_index = 0;
    ring_buffer->full = 0;
}

void user_ring_buffer_clear(ring_buffer_t * ring_buffer)
{
    ring_buffer->read_index = 0;
    ring_buffer->write_index = 0;
    ring_buffer->full = 0;
}

uint32_t user_ring_buffer_bytes_avaiable(ring_buffer_t * ring_buffer)
{
    if(ring_buffer->full) return ring_buffer->size;
    int uilen = ring_buffer->write_index  - ring_buffer->read_index;
    if(uilen >= 0) return uilen;
    return uilen + ring_buffer->size;
}

uint8_t user_ring_buffer_empty(ring_buffer_t * ring_buffer)
{
    return (user_ring_buffer_bytes_avaiable(ring_buffer) == 0);
}

uint32_t user_ring_buffer_bytes_free(ring_buffer_t * ring_buffer)
{
    return ring_buffer->size - user_ring_buffer_bytes_avaiable(ring_buffer);
}

int user_ring_buffer_write(ring_buffer_t * ring_buffer,uint8_t *data,uint32_t length)
{
    if(user_ring_buffer_bytes_free(ring_buffer) < length){
        return ERROR_RING_BUFFE_OVERFLOW;
    }
    if(length == 0) return ERROR_RING_BUFFER_LEN;
 
    uint32_t bytes_end = ring_buffer->size - ring_buffer->write_index;
    uint32_t bytes_to_copy = MIN(bytes_end,length);
    memcpy(&ring_buffer->stroage[ring_buffer->write_index],data,bytes_to_copy);
    length -= bytes_to_copy;
    ring_buffer->write_index = (ring_buffer->write_index + bytes_to_copy) % ring_buffer->size;
    if(length){
        data += bytes_to_copy;
        memcpy(&ring_buffer->stroage[0],data,length);
        ring_buffer->write_index += length;
    }
    if(ring_buffer->write_index == ring_buffer->read_index){
        ring_buffer->full = 1;
    }
    return ERROR_NO;
}

int user_ring_buffer_read(ring_buffer_t * ring_buffer,uint8_t *data,uint32_t length)
{
    uint32_t uilen = 0;
	uint32_t bytes = user_ring_buffer_bytes_avaiable(ring_buffer);
    length = MIN(length,bytes);
    uilen = length;
    if(length == 0) return 0;

    uint32_t bytes_end = ring_buffer->size - ring_buffer->read_index;
    uint32_t bytes_to_copy = MIN(bytes_end,length);
    memcpy(data,&ring_buffer->stroage[ring_buffer->read_index],bytes_to_copy);
    length -= bytes_to_copy;
    data += bytes_to_copy;
    ring_buffer->read_index = (ring_buffer->read_index + bytes_to_copy) % ring_buffer->size;

    if(length){
        memcpy(data,&ring_buffer->stroage[0],length);
        ring_buffer->read_index += length;
    }
    ring_buffer->full = 0;
    return uilen;
    
}

