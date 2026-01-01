
#include "ice_packet_pool.h"
#include <stddef.h>

static ice_frame_buffer_t frame_buffer_pool[POOL_SIZE];

ice_frame_buffer_t *ice_alloc() {
    for (int i = 0; i < POOL_SIZE; i++) {
        if (frame_buffer_pool[i].state == BUFFER_FREE) {
            frame_buffer_pool[i].state  = BUFFER_BUSY;
            frame_buffer_pool[i].length = 0;
            return &frame_buffer_pool[i];
        }
    }

    return NULL;
}

void ice_free(ice_frame_buffer_t *buffer_ptr) {
    buffer_ptr->state = BUFFER_FREE;
}