#include "ice_comms_engine.h"
#include "ice_packetizer.h"
#include "ice_transport_usb.h"

static ice_transport_t usb_transport;
static ice_packetizer_ctx_t unpacketizer_ctx;
static ice_packetizer_ctx_t packetizer_ctx;

static ice_comms_frame_queue_t rx_queue;
static ice_comms_frame_queue_t tx_queue;

static ice_comms_handler_t rx_handler = NULL;

void ice_comms_init(ice_comms_handler_t handler_fn_rx) {
    ice_transport_usb_init(&usb_transport);

    ice_frame_q_init(&rx_queue);
    ice_frame_q_init(&tx_queue);

    ice_unpacketizer_ctx_init(&unpacketizer_ctx, &rx_queue);
    ice_packetizer_ctx_init(&packetizer_ctx, &tx_queue);

    rx_handler = handler_fn_rx;

}

// TODO: implement CRC check at [1]
// TODO: implement error handler at [2], [3]
void ice_comms_run(void) {
    ice_unpacketize(&usb_transport, &unpacketizer_ctx);
    ice_packetize(&usb_transport, &packetizer_ctx);

    ice_frame_buffer_t *rx_frame;

    if (ICE_FQ_OK == ice_frame_q_pop(&rx_queue, &rx_frame)) {
        if (rx_handler != NULL) {
            rx_handler(rx_frame->buffer, rx_frame->length, &tx_queue);
        } 
        ice_free(rx_frame);
    }

}