#include "ice_comms_engine.h"
#include "ice_delay.h"
#include "ice_command_handler.h"
#include "main.h"
#include "stm32h7xx_ll_gpio.h"



int bootloader(void) {
    ICE_delay_init();
    ice_comms_init(ice_command_handler);

    uint32_t start_tick = ICE_get_tick();
    while (1) {
        ice_comms_run();

        if (ICE_get_tick() - start_tick > 500){
            LL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
            start_tick = ICE_get_tick();
        }
    }
}