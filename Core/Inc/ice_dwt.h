#ifndef ICE_DWT_H
#define ICE_DWT_H

#include "stm32h743xx.h"

#define ICE_DWT_init()                                  \
    do {                                                \
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; \
    } while (0)

#define ICE_DWT_reset_cycle_count() \
    do {                            \
        DWT->CYCCNT = 0;            \
    } while (0)

#define ICE_DWT_enable_cycle_counting()      \
    do {                                     \
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; \
    } while (0)

#define ICE_DWT_get_cycle_count() (DWT->CYCCNT)

#define ICE_DWT_disable_cycle_counting()      \
    do {                                      \
        DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; \
    } while (0)

#endif // ICE_DWT_H