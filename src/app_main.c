#include "ice_comms_engine.h"
#include "ice_delay.h"
#include "ice_command_handler.h"



int bootloader(void) {
    ICE_delay_init();
    ice_comms_init(ice_command_handler);


    while (1) {
        ice_comms_run();
    }
}