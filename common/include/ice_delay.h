#ifndef ICE_DELAY_H
#define ICE_DELAY_H

#include <stdint.h>

#define ICE_US_PER_SECOND 1000000

void ICE_delay_init(void);
 
void ICE_delay_us(uint32_t us);

void ICE_delay_ms(uint32_t ms);

uint32_t ICE_get_tick();

void ICE_tick_inc();


#endif // ICE_DELAY_H