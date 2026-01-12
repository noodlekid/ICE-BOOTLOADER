#ifndef ICE_BIT_UTILITIES_H
#define ICE_BIT_UTILITIES_H

#include <stdint.h>

static inline uint32_t ice_extract_uint32_be(const uint8_t *buffer){
    return (uint32_t)buffer[0] | 
            (uint32_t)buffer[1] << 8 |
            (uint32_t)buffer[2] << 16 |
            (uint32_t)buffer[3] << 24;
}

static inline uint32_t ice_extract_uint32_le(const uint8_t *buffer){
    return (uint32_t)buffer[3] |
            (uint32_t)buffer[2] << 8 |
            (uint32_t)buffer[1] << 16 | 
            (uint32_t)buffer[0];
}

#define READ_BIT(REG, BIT) ((REG) & (BIT))
#define SET_BIT(REG, BIT) ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT) ((REG) &= ~(BIT))


#endif // ICE_BIT_UTILITIES_H