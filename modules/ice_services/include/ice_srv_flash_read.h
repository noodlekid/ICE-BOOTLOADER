#ifndef ICE_SRV_FLASH_READ_H
#define ICE_SRV_FLASH_READ_H

#include "ice_boot_protocol.h"

ice_service_status_t ice_srv_flash_read(ice_boot_pdu *pdu, uint8_t* resp_buf, uint32_t* resp_len);

#endif