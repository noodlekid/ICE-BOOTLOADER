#ifndef ICE_RING_BUFFER_H
#define ICE_RING_BUFFER_H

#include <stdint.h>

#define RING_BUFFER_SIZE 2048

typedef struct {
    uint8_t buffer[RING_BUFFER_SIZE];
    volatile uint16_t head;
    volatile uint16_t tail;
} ICE_COMMON_ring_buffer_t;

uint8_t ICE_COMMON_ring_buffer_pop(ICE_COMMON_ring_buffer_t* rb);
void ICE_COMMON_ring_buffer_put(ICE_COMMON_ring_buffer_t* rb, uint8_t data);

#endif  // ICE_RING_BUFFER_H