#ifndef ICE_COMMS_STRUCTURES_H
#define ICE_COMMS_STRUCTURES_H

#include <stdint.h>

#define MTU_SIZE 256 /* maximum transmition unit size */


/* contains a single decoded frame */
typedef struct {
    uint8_t frame_len;
    uint8_t frame_buffer[MTU_SIZE];
} ice_data_frame_t;


#endif // ICE_COMMS_STRUCTURES_H