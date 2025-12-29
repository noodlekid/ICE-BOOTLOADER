#ifndef ICE_COMMS_SERVICE_H
#define ICE_COMMS_SERVICE_H

#include <stdint.h>

#define MAX_FRAME_LENGTH 258 /* [BODY - 256][CRC - 2] */

typedef enum {
    PACKET_CRC_ERROR   = -1,
    PACKET_IN_PROGRESS = 0,
    PACKET_READY       = 1,
} ice_packet_status_t;
typedef struct {
    uint32_t timeout_ticks;
    uint32_t rx_start_tick;
    uint8_t state;
    uint8_t frame_buffer[MAX_FRAME_LENGTH];
    uint8_t counter;
    uint8_t expected_len;
} ice_comms_ctx_t;

void ice_comms_init(ice_comms_ctx_t *ctx);
ice_packet_status_t ice_process_byte(ice_comms_ctx_t *ctx, uint8_t byte);

#endif // ICE_COMMS_SERVICE_H