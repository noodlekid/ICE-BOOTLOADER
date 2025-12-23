#ifndef DELAY_H
#define DELAY_H

#include "ice_dwt.h"

#define ICE_US_PER_SECOND 1000000

static inline void ICE_delay_init(void) {
    ICE_DWT_init();
    ICE_DWT_reset_cycle_count();
    ICE_DWT_enable_cycle_counting();
}

static inline void ICE_delay_us(uint32_t us) {
    uint32_t start_cycles      = ICE_DWT_get_cycle_count();
    uint64_t delay_by_x_cycles = ((uint64_t)SystemCoreClock / ICE_US_PER_SECOND) * us;
    while ((ICE_DWT_get_cycle_count() - start_cycles) < delay_by_x_cycles)
        ;
}
static inline void ICE_delay_ms(uint32_t ms) {
    ICE_delay_us((uint64_t)ms * 1000);
}

#endif // DELAY_H