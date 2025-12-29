#ifndef ICE_BOOT_PROTOCOL
#define ICE_BOOT_PROTOCOL

#include <stdint.h>

#define ICE_MAX_PAYLOAD_SIZE 256

typedef enum {
    PING        = 0x00,
    FLASH_ERASE = 0x01,
    FLASH_READ  = 0x02,
    FLASH_WRITE = 0x03,
    BOOT        = 0x04,
} ice_boot_codes_t;

typedef struct __attribute__((packed)) {
    uint8_t op;       /* PING, ERASE, READ, WRITE */
    uint8_t seq_num;  /* packet ID for identifying duplicates */
    uint8_t flags;    /* status bits for errors and such */
    uint8_t _packing; /* alignment byte */
    uint32_t arg;     /* generic argument, simplifies further logic */
    uint32_t length;  /* describes paylaod length */
} ice_boot_header_t;

typedef struct __attribute__((packed)) {
    ice_boot_header_t header;
    uint8_t payload[ICE_MAX_PAYLOAD_SIZE];
    uint16_t crc;
} ice_boot_pdu_t;

typedef union {
    uint8_t raw[sizeof(ice_boot_pdu_t)];
    ice_boot_pdu_t packet;
} ice_boot_pdu;

#endif