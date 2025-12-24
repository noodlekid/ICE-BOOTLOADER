#ifndef ICE_RING_BUFFER_H
#define ICE_RING_BUFFER_H

#include <stdint.h>
typedef enum {
    ICE_COMMON_RB_FULL,
    ICE_COMMON_RB_EMPTY,
    ICE_COMMON_RB_NULL_ERR,
    ICE_COMMON_NULL_DATA_ERR,
    ICE_COMMON_RB_SIZE_MISMATCH,
    ICE_COMMON_RB_OK,
} ICE_COMMON_RB_status_t;

typedef struct {
    volatile uint16_t head;
    volatile uint16_t tail;
    uint16_t size_mask;
    uint8_t *buffer;
} ICE_COMMON_ring_buffer_t;

ICE_COMMON_RB_status_t ICE_COMMON_ring_buffer_init(ICE_COMMON_ring_buffer_t* rb, uint8_t* buffer, uint16_t size);
ICE_COMMON_RB_status_t ICE_COMMON_ring_buffer_pop(ICE_COMMON_ring_buffer_t* rb, uint8_t *data);
ICE_COMMON_RB_status_t ICE_COMMON_ring_buffer_put(ICE_COMMON_ring_buffer_t* rb, uint8_t data);

#endif  // ICE_RING_BUFFER_H