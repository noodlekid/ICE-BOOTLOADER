#ifndef ICE_RING_BUFFER_H
#define ICE_RING_BUFFER_H

#include <stdint.h>

#ifdef ICE_RB_USE_CONF
    #include "ice_ring_buffer_conf.h"
#endif

#ifndef ICE_RB_MEMORY_BARRIER
    #define ICE_RB_MEMORY_BARRIER() __asm volatile("" ::: "memory")
#endif



typedef enum {
    ICE_RB_FULL,
    ICE_RB_EMPTY,
    ICE_RB_NULL_ERR,
    ICE_NULL_DATA_ERR,
    ICE_RB_SIZE_MISMATCH,
    ICE_RB_OK,
} ice_rb_status_t;

typedef struct {
    volatile uint16_t head;
    volatile uint16_t tail;
    uint16_t size_mask;
    uint8_t *buffer;
} ice_ring_buffer_t;

ice_rb_status_t ice_ring_buffer_init(ice_ring_buffer_t* rb, uint8_t* buffer, uint16_t size);
ice_rb_status_t ice_ring_buffer_pop(ice_ring_buffer_t* rb, uint8_t *data);
ice_rb_status_t ice_ring_buffer_put(ice_ring_buffer_t* rb, uint8_t data);
ice_rb_status_t ice_ring_buffer_put_chunk(ice_ring_buffer_t* rb, const uint8_t *data, uint16_t len);
ice_rb_status_t ice_ring_buffer_pop_chunk(ice_ring_buffer_t *rb, uint8_t *chunk, uint16_t len);

#endif  // ICE_RING_BUFFER_H