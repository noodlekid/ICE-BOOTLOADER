#ifndef ICE_CRC_H
#define ICE_CRC_H

#include <stdint.h>

uint16_t ice_crc_calcuate(uint8_t* data, uint16_t len);

#endif // ICE_CRC_H