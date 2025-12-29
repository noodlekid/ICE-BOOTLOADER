#include "ice_boot_protocol.h"
#include "ice_comms_service.h"
#include "ice_delay.h"
#include "ice_transport_usb.h"
#include "main.h"
#include "stm32h7xx_ll_gpio.h"
#include <string.h>

static uint8_t ok[]   = {'O', 'K'};
static uint8_t nack[] = {'N', 'A', 'C', 'K'};

int bootloader(void) {
    ICE_delay_init();

    ICE_TRANSPORT_t usb_transport;
    ice_comms_ctx_t comms_ctx;

    ICE_TRANSPORT_usb_init(&usb_transport);
    ice_comms_init(&comms_ctx);
    uint8_t rx_byte;
    ice_packet_status_t status;
    uint32_t start = ICE_get_tick();
    uint32_t toggle_speed = 500;

    ice_boot_pdu_t packet_buffer[4];
    
    
    while (1) {

        /* comms */
        if (usb_transport.ops->read(&usb_transport, &rx_byte, 1) > 0) {
            /* there are bytes awaiting processing, begin decoding (COBS) */
            status = ice_process_byte(&comms_ctx, rx_byte);
            if (status == PACKET_READY){
                /* PDU ready to be passed around like riley reid */
                /* copy packet from context frame buffer to the packet buffer (figure out how to manage boundaries?) */
            }

        }
        

        /* debug indicator, making sure we havent halted */
        if (ICE_get_tick() - start > toggle_speed) {
            LL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
            start = ICE_get_tick();
        }
    }
}