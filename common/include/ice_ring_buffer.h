#ifndef ICE_RING_BUFFER_H
#define ICE_RING_BUFFER_H

#include <stdint.h>
typedef enum {
    ICE_RB_FULL,
    ICE_RB_EMPTY,
    ICE_RB_NULL_ERR,
    ICE_NULL_DATA_ERR,
    ICE_RB_SIZE_MISMATCH,
    ICE_RB_OK,
} ICE_RB_status_t;

typedef struct {
    volatile uint16_t head;
    volatile uint16_t tail;
    uint16_t size_mask;
    uint8_t *buffer;
} ICE_ring_buffer_t;

ICE_RB_status_t ICE_ring_buffer_init(ICE_ring_buffer_t* rb, uint8_t* buffer, uint16_t size);
ICE_RB_status_t ICE_ring_buffer_pop(ICE_ring_buffer_t* rb, uint8_t *data);
ICE_RB_status_t ICE_ring_buffer_put(ICE_ring_buffer_t* rb, uint8_t data);
ICE_RB_status_t ICE_ring_buffer_put_many(ICE_ring_buffer_t* rb, const uint8_t *data, uint16_t len);

#endif  // ICE_RING_BUFFER_H