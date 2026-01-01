#include "ice_comms_frame_queue.h"
#include "ice_packet_pool.h"

ice_comms_frame_queue_status_t ice_frame_q_init(ice_comms_frame_queue_t *fq) {
    if (fq == NULL) {
        return ICE_FQ_NULL_ERR;
    }

    fq->head = 0;
    fq->tail = 0;

    return ICE_FQ_OK;
}
ice_comms_frame_queue_status_t ice_frame_q_put(ice_comms_frame_queue_t *fq,
                                               ice_frame_buffer_t *fb_ptr) {
    if (fq == NULL) {
        return ICE_FQ_NULL_ERR;
    }

    uint8_t new_head = (fq->head + 1) & (MAX_Q_SIZE - 1);
    if (new_head == fq->tail) {
        return ICE_FQ_FULL;
    }

    fq->frames[fq->head] = fb_ptr;
    fq->head             = new_head;

    return ICE_FQ_OK;
}
ice_comms_frame_queue_status_t ice_frame_q_pop(ice_comms_frame_queue_t *fq,
                                               ice_frame_buffer_t **ret_buffer) {
    if (fq == NULL) {
        return ICE_FQ_NULL_ERR;
    }

    if (fq->tail == fq->head) {
        return ICE_FQ_EMPTY;
    }

    *ret_buffer = fq->frames[fq->tail];
    fq->tail    = (fq->tail + 1) & (MAX_Q_SIZE - 1);

    return ICE_FQ_OK;
}
