#include "ice_bit_utilities.h"
#include "ice_boot_protocol.h"
#include "ice_flash.h"
#include <stddef.h>

ice_service_status_t ice_srv_flash_erase(ice_boot_pdu *pdu, uint8_t *resp_buf, uint16_t *resp_len) {
    if (resp_buf == NULL || resp_len == NULL) {
        return ICE_FAIL;
    }

    uint32_t start_addr = ice_extract_uint32_be(pdu->packet.payload);
    uint32_t len        = ice_extract_uint32_be(pdu->packet.payload + 4);

    if (ICE_FLASH_OK != ice_flash_erase_range(start_addr, len)) {
        return ICE_FAIL;
    }

    return ICE_OK;
}