
#include "ice_crc.h"
#include "stm32h743xx.h"


uint16_t ice_crc_calcuate(uint8_t *data, uint16_t len) {
    CRC->CR |= CRC_CR_RESET;
    for (int i = 0; i < len; i++) {
        *(uint8_t*)(&CRC->DR) = data[i];
    }
    return (uint16_t)CRC->DR;
}