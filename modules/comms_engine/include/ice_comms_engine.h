#ifndef ICE_COMMS_ENGINE_H
#define ICE_COMMS_ENGINE_H

#include <stdint.h>

typedef void (*ice_comms_handler_t)(uint8_t *data, uint16_t len);

void ice_comms_init(ice_comms_handler_t handler_fn);
void ice_comms_run(void);

#endif  // ICE_COMMS_ENGINE_H