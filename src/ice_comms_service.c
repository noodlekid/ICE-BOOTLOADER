
#include "ice_crc.h"
#include "ice_comms_service.h"
#include <stdint.h>
#include "ice_delay.h"

typedef enum { STATE_IDLE = 0, STATE_LEN, STATE_BODY } ice_comms_state_t;

#define SOF (0xAAU)

static void ice_reset_state(ice_comms_ctx_t *ctx) {
    ctx->state        = STATE_IDLE;
    ctx->counter      = 0;
    ctx->expected_len = 0;
}

void ice_comms_init(ice_comms_ctx_t *ctx) {
    ice_reset_state(ctx);
}

ice_packet_status_t ice_process_byte(ice_comms_ctx_t *ctx, uint8_t byte) {

    if ((ctx->state != STATE_IDLE) && ICE_get_tick() - ctx->rx_start_tick > ctx->timeout_ticks){
        ice_reset_state(ctx);
    }

    switch (ctx->state) {
    case STATE_IDLE:
        if (byte == SOF) {
            /* first valid byte update timer */
            ctx->rx_start_tick = ICE_get_tick();
            ctx->state = STATE_LEN;
        }
        break;
    case STATE_LEN:
        /* NOTE to future Mat: if you increase max length of packet (> 256) check it here */
        ctx->expected_len = byte;
        ctx->counter      = 0;
        ctx->state        = STATE_BODY;
        break;
    case STATE_BODY:
        ctx->frame_buffer[ctx->counter++] = byte;

        if (ctx->counter >= (ctx->expected_len + 2)) {

            /* full packet received at this point we can reset timer */
            uint16_t received_crc = (uint16_t)ctx->frame_buffer[ctx->expected_len] |
                                    ((uint16_t)ctx->frame_buffer[ctx->expected_len + 1] << 8);
            uint16_t expected_crc = ice_crc_calcuate(ctx->frame_buffer, ctx->expected_len);


            ice_reset_state(ctx);

            if (received_crc == expected_crc) {
                return PACKET_READY;
            } else {
                return PACKET_CRC_ERROR;
            }
        }
        break;

    default:
        ice_reset_state(ctx);
        break;
    }


    /* still processing packet here */
    return PACKET_IN_PROGRESS;
}