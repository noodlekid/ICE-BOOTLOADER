#ifndef ICE_COBS_H
#define ICE_COBS_H

#include <stddef.h>
#include <stdint.h>


/* chunked cobs encode */
size_t ice_cobs_encode(uint8_t *dst_buf_ptr, size_t dst_buf_len, const void *src_buf_ptr, size_t src_buf_len);

/* chunked cobs decode */
size_t ice_cobs_decode(void *dst_buf_ptr, size_t dst_buf_len, const uint8_t *src_buf_ptr, size_t src_buf_len);


#endif // ICE_COBS_H