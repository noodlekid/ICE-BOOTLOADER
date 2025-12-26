#ifndef CRC_H
#define CRC_H

#include "stm32h743xx.h"
#include <stdint.h>

void crc_init_16bit(uint32_t polynomial, uint32_t init_value){

    RCC->AHB4ENR |= RCC_AHB4ENR_CRCEN;
    CRC->CR |= CRC_CR_RESET;
    CRC->CR = (CRC->CR & ~CRC_CR_POLYSIZE) | (0b01 << CRC_CR_POLYSIZE_Pos);
    CRC->POL = polynomial;
    CRC->INIT = init_value;
}

#endif // CRC_H