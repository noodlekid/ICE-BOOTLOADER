#ifndef ICE_COMMAND_HANDLER_H
#define ICE_COMMAND_HANDLER_H

#include <stdint.h>
#include "ice_comms_frame_queue.h"

void ice_command_handler(uint8_t* data, uint16_t size, ice_comms_frame_queue_t* tx_queue);

#endif // ICE_COMMAND_HANDLER_H