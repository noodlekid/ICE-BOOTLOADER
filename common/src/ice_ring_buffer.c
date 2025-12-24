#include "ice_ring_buffer.h"

#include <stdint.h>
#include <stddef.h>



ICE_COMMON_RB_status_t ICE_COMMON_ring_buffer_init(ICE_COMMON_ring_buffer_t *rb, uint8_t* buffer, uint16_t size){
    if (buffer == NULL || rb == NULL){
        return ICE_COMMON_RB_NULL_ERR;
    }

    if ((size == 0) || ((size & (size - 1)) == 0)) {
        /* must be a power of base 2 */
        return ICE_COMMON_RB_SIZE_MISMATCH;
    }

    rb->buffer = buffer;
    rb->size_mask = size - 1;
    rb->head = 0;
    rb->tail = 0;

    return ICE_COMMON_RB_OK;

}


ICE_COMMON_RB_status_t ICE_COMMON_ring_buffer_put(ICE_COMMON_ring_buffer_t *rb, uint8_t data){
    if (rb == NULL) return ICE_COMMON_RB_NULL_ERR;

    /* check if we are able to put new byte in the rb*/
    uint16_t new_head = (rb->head + 1) & (rb->size_mask);

    if (new_head == rb->tail){
        return ICE_COMMON_RB_FULL;
    }

    rb->buffer[rb->head] = data;
    rb->head = new_head;

    return ICE_COMMON_RB_OK;

}


ICE_COMMON_RB_status_t ICE_COMMON_ring_buffer_pop(ICE_COMMON_ring_buffer_t* rb, uint8_t *data){
    if (rb == NULL) return ICE_COMMON_RB_NULL_ERR;

    
    if (rb->tail == rb->head){
        return ICE_COMMON_RB_EMPTY;
    }

    *data = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) & rb->size_mask;

    return ICE_COMMON_RB_OK;
}