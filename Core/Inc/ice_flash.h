#ifndef ICE_FLASH_H
#define ICE_FLASH_H

#include <stdint.h>
#include "stm32h743xx.h"

#define FLASH_REGION_BEGIN_ADDR 0x08000000
#define FLASH_REGION_END_ADDR   0x081FFFFF

#define FLASH_BANK_1_BEGIN_ADDR FLASH_REGION_BEGIN_ADDR
#define FLASH_BANK_1_END_ADDR   0x080FFFFF

#define FLASH_BANK_2_BEGIN_ADDR 0x08100000
#define FLASH_BANK_2_END_ADDR   FLASH_REGION_END_ADDR

#define FLASH_KEY_1 0x45670123U
#define FLASH_KEY_2 0xCDEF89ABU

#define FLASH_ALIGN_MASK 0x1FU

typedef enum {
    ICE_FLASH_OK,
    ICE_FLASH_ERR,
    ICE_FLASH_INVALID_BANK,
    ICE_FLASH_INVALID_SECTOR,
    ICE_FLASH_OUT_OF_RANGE_ERR,
    ICE_FLASH_MISALIGNED_WRITE_ERR,
    ICE_FLASH_WRITE_FAILURE,
} ICE_FLASH_StatusTypeDef;




static inline uint32_t ICE_FLASH_get_sector(uint32_t addr) {
    uint32_t bank_base = 0;

    if (addr >= FLASH_BANK2_BASE) {
        bank_base = FLASH_BANK2_BASE;
    } else {
        bank_base = FLASH_BANK1_BASE;
    }

    uint32_t offset = addr - bank_base;

    uint32_t sector_index = offset / FLASH_SECTOR_SIZE;

    return sector_index;
}

static inline uint32_t ICE_FLASH_get_bank(uint32_t addr){
    if (addr >= FLASH_BANK_1_BEGIN_ADDR && addr <= FLASH_BANK_1_END_ADDR) {
        return 1U;
    }
    if (addr >= FLASH_BANK_2_BEGIN_ADDR && addr <= FLASH_BANK_2_END_ADDR) {
        return 2U;
    }
    return 0U;
}

ICE_FLASH_StatusTypeDef ICE_FLASH_erase_sector(uint32_t bank_number, uint32_t sector_number);
ICE_FLASH_StatusTypeDef ICE_FLASH_erase_range(uint32_t start_addr, uint32_t length);
void ICE_FLASH_unlock_options(void);
void ICE_FLASH_unlock(void);
void ICE_FLASH_lock(void);
ICE_FLASH_StatusTypeDef ICE_FLASH_write(uint32_t address, uint32_t data_addr, uint32_t length);
ICE_FLASH_StatusTypeDef ICE_FLASH_read(uint32_t address, uint8_t *data, uint32_t length);
void ICE_FLASH_crc(uint32_t address, uint32_t length, uint32_t *crc_out);

#endif // ICE_FLASH_H