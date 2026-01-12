#ifndef ICE_SRV_PING_H
#define ICE_SRV_PING_H

#include <stdint.h>
#include "ice_boot_protocol.h"

ice_service_status_t ice_srv_ping(uint8_t *resp_buf, uint32_t *resp_len);

#endif // ICE_SRV_PING_H