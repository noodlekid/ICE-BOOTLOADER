
#include <stdint.h>

typedef struct {

    uint8_t state;
    uint8_t frame_buffer[256];
    uint8_t payload_counter;
    uint8_t expected_len;

} ice_comms_ctx_t;

typedef enum {
    STATE_IDLE = 0,
    STATE_LEN,
    STATE_PAYLOAD,
    STATE_CRC
} ice_comms_state_t;

#define SOF (0xAAU)

static void ice_reset_state(ice_comms_ctx_t* ctx){
    ctx->state = STATE_IDLE;
    ctx->payload_counter = 0;
    ctx->expected_len = 0;
}

void ice_process_byte(ice_comms_ctx_t* ctx, uint8_t byte) {

    switch(ctx->state){
        case STATE_IDLE:
            if (byte == SOF){
                ctx->payload_counter = 0;
                ctx->expected_len = 0;
                ctx->state = STATE_LEN;
            }
            break;
        case STATE_LEN:
            ctx->expected_len = byte;
            if (ctx->expected_len == 0){
                ctx->state = STATE_CRC;
            } else {
                ctx->state = STATE_PAYLOAD;
            }
            break;
        case STATE_PAYLOAD:
            ctx->frame_buffer[ctx->payload_counter] = byte;
            ctx->payload_counter++;

            if (ctx->expected_len <= ctx->payload_counter){
                ctx->state = STATE_CRC;
            }
            break;

        case STATE_CRC:
            if (byte == calc_crc(ctx->frame_buffer, ctx->payload_counter)){
                done();
            } else {
                error();
            }
            ice_reset_state(ctx);
            break;
        default:
            ice_reset_state(ctx);
            break;
    }

}