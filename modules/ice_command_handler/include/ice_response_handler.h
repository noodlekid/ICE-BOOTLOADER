#ifndef ICE_RESPONSE_HANDLER_H
#define ICE_RESPONSE_HANDLER_H

#include "ice_boot_protocol.h"
#include "ice_comms_frame_queue.h"
#include <stdint.h>
#include <stddef.h>

void ice_send_response(ice_comms_frame_queue_t* tx_frame_buffer, const ice_boot_codes_t operation, const ice_service_status_t status, void* resp_buf, size_t resp_buf_len);

#endif // ICE_RESPONSE_HANDLER_H