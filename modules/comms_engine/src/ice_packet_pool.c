
#include "ice_packet_pool.h"
#include <stddef.h>

static ice_frame_buffer_t frame_buffer_pool[POOL_SIZE];

ice_frame_buffer_t *ice_frame_alloc() {

    ice_frame_buffer_t *block = NULL;
    for (int i = 0; i < POOL_SIZE; i++) {
        if (frame_buffer_pool[i].state == BUFFER_FREE) {
            frame_buffer_pool[i].state  = BUFFER_BUSY;
            frame_buffer_pool[i].length = 0;
            block = &frame_buffer_pool[i];
            break;
        }
    }

    return block;
}

void ice_free(ice_frame_buffer_t *buffer_ptr) {
    if (buffer_ptr == NULL) return;
    buffer_ptr->state = BUFFER_FREE;
}