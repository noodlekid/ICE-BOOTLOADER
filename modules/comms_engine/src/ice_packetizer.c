#include "ice_packetizer.h"
#include "ice_cobs.h"
#include "ice_comms_frame_queue.h"
#include "ice_packet_pool.h"
#include "ice_transport.h"
#include <stddef.h>

void ice_unpacketizer_ctx_init(ice_packetizer_ctx_t *ctx, ice_comms_frame_queue_t *rx_frame_queue) {
    ctx->outbox = rx_frame_queue;
    ctx->state  = PACKETIZER_IDLE;
}

void ice_packetizer_ctx_init(ice_packetizer_ctx_t *ctx, ice_comms_frame_queue_t *tx_frame_queue) {
    ctx->outbox = tx_frame_queue;
    ctx->state  = PACKETIZER_IDLE;
}

void ice_packetize(ice_transport_t *transport, ice_packetizer_ctx_t *ctx) {
    if (transport == NULL || ctx == NULL) {
        return;
    }
    uint8_t encoded_packet[MTU_SIZE];
    uint16_t packet_size;

    if (ICE_FQ_OK == ice_frame_q_pop(ctx->outbox, &ctx->working_buffer)) {
        if (ctx->working_buffer == NULL) {
            return;
        }
        packet_size = ice_cobs_encode(encoded_packet, MTU_SIZE, ctx->working_buffer->buffer,
                                          ctx->working_buffer->length);

        transport->ops->write(transport, encoded_packet, packet_size);
        ice_free(ctx->working_buffer);
    }

    return;
}

/* reads and processes frame by frame delimited by 0x00  */
void ice_unpacketize(ice_transport_t *transport, ice_packetizer_ctx_t *ctx) {
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

            ctx->working_buffer = ice_frame_alloc();

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
                if (!ice_frame_q_put(ctx->outbox, ctx->working_buffer)) {
                    /* outbox full, drop this packet */
                    ice_free(ctx->working_buffer);
                }
                uint8_t packet_len =
                    ice_cobs_decode(ctx->working_buffer->buffer, MTU_SIZE,
                                    ctx->working_buffer->buffer, ctx->working_buffer->length);
                if (packet_len > 0) {
                    ctx->working_buffer->length = packet_len;
                }
                ctx->working_buffer = NULL;
                ctx->state          = PACKETIZER_IDLE;
                break;
            } else {
                if (ctx->working_buffer->length >= MTU_SIZE) {
                    ice_free(ctx->working_buffer);
                    ctx->working_buffer = NULL;
                    ctx->state          = PACKETIZER_IDLE;
                } else {
                    ctx->working_buffer->buffer[ctx->working_buffer->length++] = rx_byte;
                }
            }
            break;
        }
    }

    return;
}
