#include "ice_flash.h"
#include "ice_bit_utilities.h"
#include "ice_delay.h"
#include "stm32h743xx.h"
#include <string.h>

typedef struct {
    __IO uint32_t *CR;
    __I uint32_t *SR;
    __I uint32_t *KEYR;
    __O uint32_t *CCR;
} FlashBankRegs_t;

void ice_flash_unlock() {
    if (READ_BIT(FLASH->CR1, FLASH_CR_LOCK) != 0U) {
        FLASH->KEYR1 = FLASH_KEY_1;
        FLASH->KEYR1 = FLASH_KEY_2;
    }

    if (READ_BIT(FLASH->CR2, FLASH_CR_LOCK) != 0U) {
        FLASH->KEYR2 = FLASH_KEY_1;
        FLASH->KEYR2 = FLASH_KEY_2;
    }
}

void ice_flash_lock() {
    SET_BIT(FLASH->CR1, FLASH_CR_LOCK);
    SET_BIT(FLASH->CR2, FLASH_CR_LOCK);
}

static ice_flash_status_t ice_flash_get_bank_registers(uint32_t address, FlashBankRegs_t *regs) {
    if (regs == NULL) {
        return ICE_FLASH_ERR;
    }
    if (address >= FLASH_BANK_1_BEGIN_ADDR && address <= FLASH_BANK_1_END_ADDR) {
        regs->CR   = &FLASH->CR1;
        regs->SR   = &FLASH->SR1;
        regs->KEYR = &FLASH->KEYR1;
        regs->CCR  = &FLASH->CCR1;
        return ICE_FLASH_OK;
    } else if (address >= FLASH_BANK_2_BEGIN_ADDR && address <= FLASH_BANK_2_END_ADDR) {
        regs->CR   = &FLASH->CR2;
        regs->SR   = &FLASH->SR2;
        regs->KEYR = &FLASH->KEYR2;
        regs->CCR  = &FLASH->CCR2;
        return ICE_FLASH_OK;
    }

    return ICE_FLASH_OUT_OF_RANGE_ERR;
}

static ice_flash_status_t ice_flash_write256(uint32_t flash_addr, const uint8_t *data_addr,
                                             FlashBankRegs_t *regs) {
    ice_flash_status_t status = ICE_FLASH_OK;

    uint32_t timeout_ms = 100;
    uint32_t start_tick = ICE_get_tick();
    while (READ_BIT(*(regs->SR), FLASH_SR_QW)) {
        if (ICE_get_tick() - start_tick > timeout_ms)
            return ICE_FLASH_TIMEOUT_ERR;
    }

    SET_BIT(*(regs->CR), FLASH_CR_PG);

    volatile uint32_t *dest_addr = (volatile uint32_t *)flash_addr;
    const uint8_t *src_ptr       = data_addr;
    uint32_t row_index           = FLASH_NB_32BITWORD_IN_FLASHWORD;

    /* ENTER SAFE */
    uint32_t primask_bit = __get_PRIMASK();
    __disable_irq();

    __DSB();
    __ISB();

    do {
        uint32_t word_to_write;
        memcpy(&word_to_write, src_ptr, sizeof(uint32_t));
        *dest_addr = word_to_write;
        dest_addr++;
        src_ptr += sizeof(uint32_t);
        row_index--;
    } while (row_index != 0U);

    __DSB();
    __ISB();

    __set_PRIMASK(primask_bit);
    /* EXIT SAFE */

    start_tick = ICE_get_tick();
    while (READ_BIT(*(regs->SR), FLASH_SR_QW)) {
        if (ICE_get_tick() - start_tick > timeout_ms) {
            status = ICE_FLASH_TIMEOUT_ERR;
            break;
        }
    }

    CLEAR_BIT(*(regs->CR), FLASH_CR_PG);

    return status;
}

ice_flash_status_t ice_flash_write(uint32_t flash_addr, const uint8_t *data, uint32_t length) {
    /* check alignements or else stm will be angry */
    if ((flash_addr & FLASH_ALIGN_MASK) != 0U) {
        return ICE_FLASH_MISALIGNED_WRITE_ERR;
    }

    if ((length & FLASH_ALIGN_MASK) != 0u) {
        return ICE_FLASH_MISALIGNED_WRITE_ERR;
    }

    uint32_t bytes_written   = 0;
    uint32_t curr_flash_addr = flash_addr;
    const uint8_t *curr_data_addr  = data;

    /* ensure no stale data is read, clean D-cache */
    SCB_CleanDCache_by_Addr((uint32_t *)(uintptr_t)data, length);

    ice_flash_unlock();

    while (bytes_written < length) {
        FlashBankRegs_t regs;
        ice_flash_get_bank_registers(curr_flash_addr, &regs);

        if (*(regs.SR) & (FLASH_SR_OPERR | FLASH_SR_WRPERR)) {
            *(regs.CCR) |= (FLASH_SR_OPERR | FLASH_SR_WRPERR);
        }

        /* writes must be 256 bits in size (32 bytes) */
        ice_flash_write256(curr_flash_addr, curr_data_addr, &regs);
        if (*(regs.SR) & (FLASH_SR_PGSERR | FLASH_SR_INCERR | FLASH_SR_WRPERR)) {
            ice_flash_lock();
            return ICE_FLASH_WRITE_FAILURE;
        }


        /* increment 256 bit (32 bytes) */
        bytes_written += 32;
        curr_flash_addr += 32;
        curr_data_addr += 32;
    }

    ice_flash_lock();
    return ICE_FLASH_OK;
}

ice_flash_status_t ice_flash_read(uint32_t address, uint8_t *data, uint32_t length) {
    if (address < FLASH_REGION_BEGIN_ADDR || address > FLASH_REGION_END_ADDR ||
        ((address + length) > FLASH_REGION_END_ADDR + 1)) {
        return ICE_FLASH_OUT_OF_RANGE_ERR;
    }

    uint8_t *src = (uint8_t *)address;
    /* basically a memcpy wrapper! */
    for (int i = 0; i < length; i++) {
        data[i] = src[i];
    }

    return ICE_FLASH_OK;
}

ice_flash_status_t ice_flash_erase_sector(uint32_t bank_number, uint32_t sector_number) {
    FlashBankRegs_t reg;
    if (bank_number == 1) {
        reg.CR   = &FLASH->CR1;
        reg.SR   = &FLASH->SR1;
        reg.CCR  = &FLASH->CCR1;
        reg.KEYR = &FLASH->KEYR1;

    } else if (bank_number == 2) {
        reg.CR   = &FLASH->CR2;
        reg.SR   = &FLASH->SR2;
        reg.CCR  = &FLASH->CCR2;
        reg.KEYR = &FLASH->KEYR2;
    } else {
        return ICE_FLASH_INVALID_BANK;
    }

    if (sector_number > 7) {
        return ICE_FLASH_INVALID_SECTOR;
    }
    ice_flash_unlock();

    uint32_t cr_val = *(reg.CR);

    cr_val &= ~(FLASH_CR_SNB);
    cr_val |= (sector_number << FLASH_CR_SNB_Pos) & FLASH_CR_SNB_Msk;
    cr_val &= ~(FLASH_CR_SER);
    cr_val |= FLASH_CR_SER;

    *(reg.CR) = cr_val;

    *(reg.CR) |= FLASH_CR_START;

    while (READ_BIT(*(reg.SR), FLASH_SR_BSY))
        ;

    CLEAR_BIT(*(reg.CR), FLASH_CR_SER);

    if (*(reg.SR) & (FLASH_SR_WRPERR | FLASH_SR_PGSERR | FLASH_SR_INCERR | FLASH_SR_OPERR)) {
        return ICE_FLASH_ERR;
    }

    return ICE_FLASH_OK;
}

ice_flash_status_t ice_flash_erase_range(uint32_t start_addr, uint32_t length) {
    ice_flash_status_t status = ICE_FLASH_OK;

    uint32_t end_addr = start_addr + length - 1;

    uint32_t start_bank   = ice_flash_calculate_bank(start_addr);
    uint32_t start_sector = ice_flash_calculate_sector(start_addr);

    uint32_t end_bank   = ice_flash_calculate_bank(end_addr);
    uint32_t end_sector = ice_flash_calculate_sector(end_addr);

    if (start_bank != end_bank) {
        for (uint32_t i = start_sector; i <= 7; i++) {
            status = ice_flash_erase_sector(start_bank, i);
            if (status != ICE_FLASH_OK)
                return status;
        }

        for (uint32_t i = 0; i <= end_sector; i++) {
            status = ice_flash_erase_sector(end_bank, i);
            if (status != ICE_FLASH_OK)
                return status;
        }
    } else {
        for (uint32_t i = start_sector; i <= end_sector; i++) {
            status = ice_flash_erase_sector(start_bank, i);
            if (status != ICE_FLASH_OK)
                return status;
        }
    }

    return status;
}