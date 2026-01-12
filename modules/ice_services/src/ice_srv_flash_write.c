#include "ice_bit_utilities.h"
#include "ice_boot_protocol.h"
#include "ice_flash.h"
#include <stddef.h>

ice_service_status_t ice_srv_flash_write(ice_boot_pdu *pdu, uint8_t *resp_buf, uint32_t *resp_len) {
    if (pdu == NULL) {
        return ICE_FAIL;
    }

    uint32_t start_addr = ice_extract_uint32_be(pdu->packet.payload);
    uint8_t *data       = pdu->packet.payload + 4;
    uint16_t len        = pdu->packet.header.length - 4;

    if (ICE_FLASH_OK != ice_flash_write(start_addr, data, len)) {
        return ICE_FAIL;
    }

    return ICE_OK;
}