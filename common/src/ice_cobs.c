#include "ice_cobs.h"

size_t ice_cobs_encode(uint8_t *dst_buf_ptr, size_t dst_buf_len, const void *src_buf_ptr,
                       size_t src_buf_len) {
    uint8_t *dst_end = dst_buf_ptr + dst_buf_len;

    if (dst_buf_ptr >= dst_end) {
        return 0;
    }

    uint8_t *encode   = dst_buf_ptr;
    uint8_t *code_ptr = encode++;
    uint8_t code      = 1;
    for (const uint8_t *byte = (const uint8_t *)src_buf_ptr; src_buf_len--; ++byte) {
        if (*byte) {
            if (encode >= dst_end)
                return 0;
            *encode++ = *byte, ++code;
        }

        if (!*byte || code == 0xFF) {
            *code_ptr = code, code = 1, code_ptr = encode;
            if (!*byte || src_buf_len) {
                if (encode >= dst_end)
                    return 0;
                ++encode;
            }
        }
    }
    *code_ptr = code;

    return (size_t)(encode - dst_buf_ptr);
}

size_t ice_cobs_decode(void *dst_buf_ptr, size_t dst_buf_len, const uint8_t *src_buf_ptr,
                   size_t src_len) {
    if (dst_buf_ptr == NULL || src_buf_ptr == NULL)
        return 0;

    const uint8_t *byte = src_buf_ptr;
    uint8_t *decode     = dst_buf_ptr;
    uint8_t *dst_end = dst_buf_ptr + dst_buf_len;

    for (uint8_t code = 0xFF, block = 0; byte < src_buf_ptr + src_len; --block) {
        if (block) {
            if (decode >= dst_end) return 0;
            *decode++ = *byte++;
        } else {
            block = *byte++;
            if (block && (code != 0xFF)) {
                *decode++ = 0;
            }
            code = block;
            if (!code) {
                break;
            }
        }
    }

    return (size_t)(decode - (uint8_t *)dst_buf_ptr);
}