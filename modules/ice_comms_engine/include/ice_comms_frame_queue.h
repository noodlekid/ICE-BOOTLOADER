#ifndef ICE_COMMS_FRAME_QUEUE_H
#define ICE_COMMS_FRAME_QUEUE_H

#include "ice_packet_pool.h"
#include <stddef.h>
#include <stdint.h>

#define MAX_Q_SIZE POOL_SIZE

typedef struct {
    ice_frame_buffer_t *frames[POOL_SIZE];
    volatile uint8_t head;
    volatile uint8_t tail;
} ice_comms_frame_queue_t;

typedef enum {
    ICE_FQ_FULL,
    ICE_FQ_EMPTY,
    ICE_FQ_NULL_ERR,
    ICE_NULL_DATA_ERR,
    ICE_FQ_SIZE_MISMATCH,
    ICE_FQ_OK
} ice_comms_frame_queue_status_t;

ice_comms_frame_queue_status_t ice_frame_q_init(ice_comms_frame_queue_t *fq);
ice_comms_frame_queue_status_t ice_frame_q_put(ice_comms_frame_queue_t *fq,
                                               ice_frame_buffer_t *fb_ptr);
ice_comms_frame_queue_status_t ice_frame_q_pop(ice_comms_frame_queue_t *fq,
                                               ice_frame_buffer_t **ret_buffer);

#endif // ICE_COMMS_FRAME_QUEUE_H