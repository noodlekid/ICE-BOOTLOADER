#include "ice_command_handler.h"
#include "ice_boot_protocol.h"
#include "ice_response_handler.h"
#include "ice_srv_flash_read.h"
#include "ice_srv_ping.h"
#include <stddef.h>

void ice_command_handler(uint8_t *data, uint16_t size, ice_comms_frame_queue_t *tx_queue) {
    if (data == NULL || size == 0) {
        return;
    }

    uint8_t resp_buff[256];
    uint32_t resp_len;
    ice_service_status_t status = ICE_FAIL;

    ice_boot_pdu_t *pdu = (ice_boot_pdu_t *)data;

    switch (pdu->header.op) {
    case PING:
        status = ice_srv_ping(resp_buff, &resp_len);
        ice_send_response(tx_queue, PING, status, resp_buff, resp_len);
        break;
    case FLASH_ERASE:
        break;
    case FLASH_READ:
        status = ice_srv_flash_read(pdu, resp_buff, &resp_len);
        ice_send_response(tx_queue, FLASH_READ, status, resp_buff, resp_len);
        break;
    case FLASH_WRITE:
        break;
    case BOOT:
        break;
    default:
        break;
    }
}