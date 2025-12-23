#include "ice_flash.h"
#include "stm32h743xx.h"
#include "utility.h"

typedef struct {
    __IO uint32_t *CR;
    __I uint32_t *SR;
    __I uint32_t *KEYR;
    __O uint32_t *CCR;
} FlashBankRegs_t;

void ICE_FLASH_unlock() {
    if (READ_BIT(FLASH->CR1, FLASH_CR_LOCK) != 0U) {
        FLASH->KEYR1 = FLASH_KEY_1;
        FLASH->KEYR1 = FLASH_KEY_2;
    }

    if (READ_BIT(FLASH->CR2, FLASH_CR_LOCK) != 0U) {
        FLASH->KEYR2 = FLASH_KEY_1;
        FLASH->KEYR2 = FLASH_KEY_2;
    }
}

void ICE_FLASH_lock() {
    SET_BIT(FLASH->CR1, FLASH_CR_LOCK);
    SET_BIT(FLASH->CR2, FLASH_CR_LOCK);
}

static ICE_FLASH_StatusTypeDef ICE_FLASH_get_bank_registers(uint32_t address,
                                                            FlashBankRegs_t *regs) {
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

static void ICE_FLASH_write256(uint32_t flash_addr, const uint32_t data_addr,
                               FlashBankRegs_t *regs) {
    SET_BIT(*(regs->CR), FLASH_CR_PG);

    uint8_t row_index            = FLASH_NB_32BITWORD_IN_FLASHWORD;
    volatile uint32_t *dest_addr = (volatile uint32_t *)flash_addr;
    volatile uint32_t *src_addr  = (volatile uint32_t *)data_addr;

    __DSB();
    __ISB();

    do {
        *dest_addr = *src_addr;
        dest_addr++;
        src_addr++;
        row_index--;
    } while (row_index != 0U);

    __DSB();
    __ISB();

    while (READ_BIT(*(regs->SR), FLASH_SR_QW))
        ;

    CLEAR_BIT(*(regs->CR), FLASH_CR_PG);
}

ICE_FLASH_StatusTypeDef ICE_FLASH_write(uint32_t flash_addr, const uint32_t data_addr,
                                        uint32_t length) {
    /* check alignements or else stm will be angry */
    if ((flash_addr & FLASH_ALIGN_MASK) != 0U) {
        return ICE_FLASH_MISALIGNED_WRITE_ERR;
    }

    if ((length & FLASH_ALIGN_MASK) != 0u) {
        return ICE_FLASH_MISALIGNED_WRITE_ERR;
    }

    uint32_t bytes_written   = 0;
    uint32_t curr_flash_addr = flash_addr;
    uint32_t curr_data_addr  = data_addr;

    /* ensure no stale data is read, clean D-cache */
    SCB_CleanDCache_by_Addr((uint32_t *)data_addr, length);

    ICE_FLASH_unlock();

    while (bytes_written < length) {
        FlashBankRegs_t regs;
        ICE_FLASH_get_bank_registers(curr_flash_addr, &regs);

        if (*(regs.SR) & (FLASH_SR_OPERR | FLASH_SR_WRPERR)) {
            *(regs.CCR) |= (FLASH_SR_OPERR | FLASH_SR_WRPERR);
        }

        /* writes must be 256 bits in size (32 bytes) */
        ICE_FLASH_write256(curr_flash_addr, curr_data_addr, &regs);
        if (*(regs.SR) & (FLASH_SR_PGSERR | FLASH_SR_INCERR | FLASH_SR_WRPERR)) {
            ICE_FLASH_lock();
            return ICE_FLASH_WRITE_FAILURE;
        }

        bytes_written += 32;
        curr_flash_addr += 32;
        curr_data_addr += 32;
    }


    ICE_FLASH_lock();
    return ICE_FLASH_OK;
}

ICE_FLASH_StatusTypeDef ICE_FLASH_read(uint32_t address, uint8_t *data, uint32_t length) {
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

ICE_FLASH_StatusTypeDef ICE_FLASH_erase_sector(uint32_t bank_number, uint32_t sector_number) {
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

    ICE_FLASH_unlock();

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

ICE_FLASH_StatusTypeDef ICE_FLASH_erase_range(uint32_t start_addr, uint32_t length) {
    ICE_FLASH_StatusTypeDef status = ICE_FLASH_OK;

    uint32_t end_addr = start_addr + length - 1;

    uint32_t start_bank   = ICE_FLASH_get_bank(start_addr);
    uint32_t start_sector = ICE_FLASH_get_sector(start_addr);

    uint32_t end_bank     = ICE_FLASH_get_bank(end_addr);
    uint32_t end_sector   = ICE_FLASH_get_sector(end_addr);

    if (start_bank != end_bank) {
        for (uint32_t i = start_sector; i <= 7; i++) {
            status = ICE_FLASH_erase_sector(start_bank, i);
            if (status != ICE_FLASH_OK) return status;
        }

        for (uint32_t i = 0; i <= end_sector; i++) {
            status = ICE_FLASH_erase_sector(end_bank, i);
            if (status != ICE_FLASH_OK) return status;
        }
    }
    else {
        for (uint32_t i = start_sector; i <= end_sector; i++) {
            status = ICE_FLASH_erase_sector(start_bank, i);
            if (status != ICE_FLASH_OK) return status;
        }
    }

    return status;
}