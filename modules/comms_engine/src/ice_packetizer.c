#include "ice_packet_pool.h"
#include "ice_transport.h"
#include <stddef.h>
#include "ice_packetizer.h"
#include "ice_comms_frame_queue.h"


void ice_packetizer_ctx_init(ice_packetizer_ctx_t* ctx, ice_comms_frame_queue_t* frame_queue){
    ctx->outbox = frame_queue;
    ctx->state = PACKETIZER_IDLE;
}


/* reads and processes frame by frame delimited by 0x00  */
void ice_packetize(ice_transport_t *transport, ice_packetizer_ctx_t *ctx) {
    /* get a free buffer to start filling up */
    if (transport == NULL || ctx == NULL) {
        return;
    }

    uint8_t rx_byte;

    /* FUTURE MAT: make this read in chunks */
    while (transport->ops->read(transport, &rx_byte, 1) > 0) {
        switch (ctx->state) {
        case PACKETIZER_IDLE:
            if (rx_byte == 0x00) {
                /* empty frame, or cut off drop packet */
                break;
            }

            ctx->working_buffer = ice_alloc();

            if (ctx->working_buffer == NULL) {
                /* unable to alocate buffer space, none available */
                break;
            }
            ctx->working_buffer->buffer[ctx->working_buffer->length++] = rx_byte;
            ctx->working_buffer->state                                 = BUFFER_BUSY;
            ctx->state                                                 = PACKETIZER_IN_PROGRESS;
            break;

        case PACKETIZER_IN_PROGRESS:
            if (rx_byte == 0x00) {
                /* packet done! puts and terminate */
                ctx->working_buffer->state = BUFFER_READY;
                if (!ice_frame_q_put(ctx->outbox, ctx->working_buffer)){
                    /* outbox full, drop this packet */
                    ice_free(ctx->working_buffer);
                }
                ctx->working_buffer        = NULL;
                ctx->state                 = PACKETIZER_IDLE;
                break;
            } else {
                if (ctx->working_buffer->length >= MTU_SIZE){
                    ice_free(ctx->working_buffer);
                    ctx->working_buffer = NULL;
                    ctx->state = PACKETIZER_IDLE;
                } else {
                    ctx->working_buffer->buffer[ctx->working_buffer->length++] = rx_byte;
                }
            }
            break;
        }
    }

    return;
}
