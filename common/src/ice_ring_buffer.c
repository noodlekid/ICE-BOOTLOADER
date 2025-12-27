#include "ice_ring_buffer.h"

#include <stdint.h>
#include <stddef.h>
#include <string.h>



ICE_RB_status_t ICE_ring_buffer_init(ICE_ring_buffer_t *rb, uint8_t* buffer, uint16_t size){
    if (buffer == NULL || rb == NULL){
        return ICE_RB_NULL_ERR;
    }

    if ((size == 0) || ((size & (size - 1)) != 0)) {
        /* must be a power of base 2 */
        return ICE_RB_SIZE_MISMATCH;
    }

    rb->buffer = buffer;
    rb->size_mask = size - 1;
    rb->head = 0;
    rb->tail = 0;

    return ICE_RB_OK;

}


ICE_RB_status_t ICE_ring_buffer_put(ICE_ring_buffer_t *rb, uint8_t data){
    if (rb == NULL) return ICE_RB_NULL_ERR;

    /* check if we are able to put new byte in the rb*/
    uint16_t new_head = (rb->head + 1) & (rb->size_mask);

    if (new_head == rb->tail){
        return ICE_RB_FULL;
    }

    rb->buffer[rb->head] = data;
    rb->head = new_head;

    return ICE_RB_OK;

}


ICE_RB_status_t ICE_ring_buffer_pop(ICE_ring_buffer_t* rb, uint8_t *data){
    if (rb == NULL) return ICE_RB_NULL_ERR;

    
    if (rb->tail == rb->head){
        return ICE_RB_EMPTY;
    }

    *data = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) & rb->size_mask;

    return ICE_RB_OK;
}

ICE_RB_status_t ICE_ring_buffer_put_many(ICE_ring_buffer_t* rb, const uint8_t *data, uint16_t len){
    if (rb == NULL || data == NULL ) {
        return ICE_RB_NULL_ERR;
    }

    uint16_t head = rb->head;
    uint16_t tail = rb->tail;
    uint16_t size = rb->size_mask + 1;

    uint16_t free_space = (tail + size - head - 1) & rb->size_mask;

    if (len > free_space){
        return ICE_RB_FULL;
    }

    uint16_t chunk_1 = size - head;
    if (chunk_1 < len){
        chunk_1 = len;
    }

    memcpy(&rb->buffer[head], data, chunk_1);

    if (len > chunk_1){
        memcpy(&rb->buffer[0], &data[chunk_1], len - chunk_1);
    }

    rb->head = (head + len) & rb->size_mask;

    return ICE_RB_OK;
    
}