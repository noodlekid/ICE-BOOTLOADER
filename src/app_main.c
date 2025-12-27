#include "ice_comms_service.h"
#include "ice_delay.h"
#include "ice_transport_usb.h"
#include "main.h"
#include "stm32h7xx_ll_gpio.h"

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

    while (1) {
        if (usb_transport.ops->read(&usb_transport, &rx_byte, 1) > 0) {
            status = ice_process_byte(&comms_ctx, rx_byte);
            if (status == PACKET_READY) {
                /* acknoweldge */
                usb_transport.ops->write(&usb_transport, ok, sizeof(ok));
            } else if (status == PACKET_CRC_ERROR) {
                /* negative acknowledgement */
                usb_transport.ops->write(&usb_transport, nack, sizeof(nack));
            }
        }

        if (ICE_get_tick() - start > 500) {
            LL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
            start = ICE_get_tick();
        }
    }
}