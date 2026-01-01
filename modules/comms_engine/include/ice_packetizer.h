#ifndef ICE_PACKETIZER_H
#define ICE_PACKETIZER_H

#include "ice_comms_frame_queue.h"
#include "ice_transport.h"
#include "ice_packet_pool.h"

typedef enum { PACKETIZER_IN_PROGRESS, PACKETIZER_IDLE } ice_packetizer_state_t;
typedef struct {
    ice_packetizer_state_t state;
    ice_frame_buffer_t *working_buffer;
    ice_comms_frame_queue_t *outbox;
} ice_packetizer_ctx_t;

void ice_packetize(ice_transport_t *transport, ice_packetizer_ctx_t *ctx);
void ice_packetizer_ctx_init(ice_packetizer_ctx_t *ctx, ice_comms_frame_queue_t* frame_queue);

#endif // ICE_PACKETIZER_H