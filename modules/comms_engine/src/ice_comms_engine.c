#include "ice_comms_engine.h"
#include "ice_cobs.h"
#include "ice_packetizer.h"
#include "ice_transport_usb.h"

static ice_transport_t usb_transport;
static ice_packetizer_ctx_t packetizer_ctx;
static ice_comms_frame_queue_t rx_queue;
static ice_comms_handler_t cmd_handler = NULL;

void ice_comms_init(ice_comms_handler_t handler_fn) {
    ice_transport_usb_init(&usb_transport);
    ice_frame_q_init(&rx_queue);
    ice_packetizer_ctx_init(&packetizer_ctx, &rx_queue);
    cmd_handler = handler_fn;
}

// TODO: implement CRC check at [1]
// TODO: implement error handler at [2]
void ice_comms_run(void) {
    ice_packetize(&usb_transport, &packetizer_ctx);
    ice_frame_buffer_t *frame;

    while (ICE_FQ_OK == ice_frame_q_pop(&rx_queue, &frame)) {
        uint8_t packet_len = ice_cobs_decode(frame->buffer, MTU_SIZE, frame->buffer, frame->length);
        if (packet_len > 0) {
            frame->length = packet_len;
            /* TODO: [1] */
            if (cmd_handler != NULL) {
                cmd_handler(frame->buffer, frame->length);
            }
        } else {
            /* TODO: [2]*/
        }

        ice_free(frame);
    }
}