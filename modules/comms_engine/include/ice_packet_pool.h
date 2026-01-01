#ifndef ICE_PACKET_POOL_H
#define ICE_PACKET_POOL_H

#include <stdint.h>
#include "ice_comms_structures.h"

#define POOL_SIZE 4


typedef enum {
    BUFFER_FREE = 0,
    BUFFER_BUSY,
    BUFFER_READY,
} ice_frame_buffer_status_t;

typedef struct {
    uint8_t buffer[MTU_SIZE];
    ice_frame_buffer_status_t state;
    uint16_t length;
} ice_frame_buffer_t;


/* gets a virgin buffer from the buffer pool */
ice_frame_buffer_t* ice_alloc(void);

/* resets frame buffer state */
void ice_free(ice_frame_buffer_t* buffer);

/* gets */


#endif // ICE_PACKET_POOL_H