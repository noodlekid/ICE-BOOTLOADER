#include "ice_srv_flash_read.h"
#include "ice_bit_utilities.h"
#include "ice_boot_protocol.h"
#include "ice_flash.h"

#include <stddef.h>

ice_service_status_t ice_srv_flash_read(ice_boot_pdu *pdu, uint8_t *resp_buf, uint32_t *resp_len) {
    if (pdu == NULL) {
        return ICE_FAIL;
    }

    uint32_t start_address = ice_extract_uint32_be(pdu->packet.payload);
    uint32_t length        = ice_extract_uint32_be(pdu->packet.payload + 4);

    if(ICE_FLASH_OK != ice_flash_read(start_address, resp_buf, length)){
        return ICE_FAIL;
    }

    *resp_len = length;

    return ICE_OK;
 
}
