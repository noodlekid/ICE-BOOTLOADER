#include "ice_response_handler.h"
#include <string.h>

void ice_send_response(ice_comms_frame_queue_t* tx_frame_buffer, const ice_boot_codes_t operation, const ice_service_status_t status, void *resp_buf, size_t resp_buf_len){

    // TODO: ADD SAFETYCHECKS EVERYWHERE



    ice_frame_buffer_t* tx_frame = ice_frame_alloc();
    if (tx_frame == NULL){
        return;
    }

    ice_boot_header_t bootloader_header;
    bootloader_header.op = operation;
    bootloader_header.flags = 0b00000000;
    bootloader_header.seq_num = 0;
    bootloader_header.arg = 0;
    bootloader_header.length = resp_buf_len;

    ice_boot_pdu bootloader_packet;

    bootloader_packet.packet.header = bootloader_header;

    memcpy(bootloader_packet.packet.payload, resp_buf, resp_buf_len);
    // [1]: can we get rid of this memcpy?    
    memcpy(tx_frame->buffer, bootloader_packet.raw, sizeof(bootloader_header) + resp_buf_len);
    tx_frame->length = sizeof(bootloader_header) + resp_buf_len;
    ice_frame_q_put(tx_frame_buffer, tx_frame);

 
}