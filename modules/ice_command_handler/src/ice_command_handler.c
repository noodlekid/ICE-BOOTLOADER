#include "ice_command_handler.h"
#include "ice_srv_ping.h"
#include "ice_boot_protocol.h"
#include "main.h"
#include "stm32h7xx_ll_gpio.h"
#include <stddef.h>

void ice_command_handler(uint8_t *data, uint16_t size){
    if (data == NULL || size == 0){
        return;
    }

    uint8_t resp_buff[256];
    uint16_t resp_len;

    ice_boot_pdu_t *pdu = (ice_boot_pdu_t*)data;

    switch(pdu->header.op){
        case PING:
            ice_srv_ping(resp_buff, &resp_len);
            LL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
            break;
        case FLASH_ERASE:
            break;
        case FLASH_READ:
            break;
        case FLASH_WRITE:
            break;
        case BOOT:
            break;
        default:
            break;
    }



}