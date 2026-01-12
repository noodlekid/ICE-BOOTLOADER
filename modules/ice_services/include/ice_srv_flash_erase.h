#ifndef ICE_SRV_FLASH_ERASE_H
#define ICE_SRV_FLASH_ERASE_H

#include <stdint.h>
#include "ice_boot_protocol.h"

ice_service_status_t ice_srv_flash_erase(ice_boot_pdu *pdu, uint8_t *resp_buf, uint16_t *resp_len);

#endif // ICE_SRV_FLASH_ERASE_H