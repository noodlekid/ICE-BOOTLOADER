#include "ice_comms_service.h"
#include "ice_delay.h"
#include "ice_transport.h"
#include "ice_transport_usb.h"
#include "main.h"
#include "stm32h7xx_ll_gpio.h"

static ICE_TRANSPORT_t trans;

static const uint8_t ping_string[] = {'F', 'A', 'R', 'T', '\r', '\n'};
static const uint8_t nack[]        = {'N', 'A', 'C', 'K'};

int bootloader(void) {
    ICE_delay_init();
    ICE_TRANSPORT_usb_init(&trans);

    uint8_t rx_byte;
    ice_comms_ctx_t usb_ctx;

    uint32_t start = ICE_get_tick();
    while (1) {
        if (ICE_get_tick() - start >= 500) {
            LL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
            start = ICE_get_tick();
        }

        if (trans.ops->read(&trans, &rx_byte, 1) > 0) {
            // If process_byte returns 1, we have a valid packet sitting in buffer
            ice_packet_status_t status = ice_process_byte(&usb_ctx, rx_byte);
            if (status == PACKET_READY) {
                if (usb_ctx.frame_buffer[0] == 0xDD) {
                    trans.ops->write(&trans, ping_string, sizeof(ping_string));
                }

                ice_comms_init(&usb_ctx);
            } else if (status == PACKET_CRC_ERROR) {
                trans.ops->write(&trans, nack, sizeof(nack));
            }
        }
    }
}