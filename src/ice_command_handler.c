

#include "ice_boot_protocol.h"

void ice_command_handler(ice_boot_pdu_t* pdu){
    
    switch (pdu->header.op) {

        case PING:
            /* ping service */
            break;

        case FLASH_ERASE:
            /* flash erase service */
            break;
        
        case FLASH_READ:
            /* flash read service */
            break;

        case FLASH_WRITE:
            /* flash write service */
            break;

        case BOOT:
            /* boot service */
            break;

        default:
            break;

    }

}