#include "ice_delay.h"
#include "ice_transport.h"
#include "ice_transport_usb.h"
#include "main.h"
#include "ice_crc.h"
#include "stm32h7xx_ll_gpio.h"

static uint8_t data[4] = {0xDE, 0xAD, 0xBE, 0XEF};
static ICE_TRANSPORT_t trans;

int bootloader(void) { 
    ICE_delay_init();
    ICE_TRANSPORT_usb_init(&trans);
    uint32_t start = ICE_get_tick();
    uint16_t crc = 0;
    
    while (1) {
        
        if ((ICE_get_tick() - start) >= 500) {
            LL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
            crc = ice_crc_calcuate(data, sizeof(data));
            trans.ops->write(&trans, data, 4);
            trans.ops->write(&trans, (uint8_t*)&crc, 2);
            start = ICE_get_tick();
        }
    }
}