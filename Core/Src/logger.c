#include "logger.h"
#include "usbd_cdc_if.h" // For CDC_Transmit_FS
#include <string.h>
#include <stdio.h>

/* Ring Buffer Storage */
__attribute__((aligned(32))) 
static uint8_t log_buffer[LOG_BUFFER_SIZE];
static volatile uint32_t head = 0;
static volatile uint32_t tail = 0;
static volatile uint8_t usb_is_busy = 0;

void Logger_Init(void) {
    head = 0;
    tail = 0;
    usb_is_busy = 0;
}

int _write(int file, char *ptr, int len) {
    (void)file;

    for (int i = 0; i < len; i++) {
        uint32_t next_head = (head + 1) % LOG_BUFFER_SIZE;

        if (next_head != tail) {
            log_buffer[head] = ptr[i];
            head = next_head;
        }
    }
    return len;
}

void Logger_Process(void) {
    if (head == tail) {
        return; 
    }

    uint32_t len_to_send = 0;
    if (head > tail) {
        len_to_send = head - tail;
    } else {
        len_to_send = LOG_BUFFER_SIZE - tail;
    }

    if (len_to_send > 64) {
        len_to_send = 64;
    }
    SCB_CleanDCache_by_Addr((uint32_t*)&log_buffer[tail], len_to_send);
    uint8_t result = CDC_Transmit_FS((uint8_t*)&log_buffer[tail], len_to_send);

    if (result == USBD_OK) {
        tail = (tail + len_to_send) % LOG_BUFFER_SIZE;
    }
}