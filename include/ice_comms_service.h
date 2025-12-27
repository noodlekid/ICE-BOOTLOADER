#ifndef ICE_COMMS_SERVICE_H
#define ICE_COMMS_SERVICE_H

#include <stdint.h>

typedef enum {
    PACKET_CRC_ERROR   = -1,
    PACKET_IN_PROGRESS = 0,
    PACKET_READY       = 1,
} ice_packet_status_t;
typedef struct {
    uint8_t state;
    uint8_t frame_buffer[260];
    uint8_t counter;
    uint8_t expected_len;
} ice_comms_ctx_t;

void ice_comms_init(ice_comms_ctx_t *ctx);
ice_packet_status_t ice_process_byte(ice_comms_ctx_t *ctx, uint8_t byte);

#endif // ICE_COMMS_SERVICE_H