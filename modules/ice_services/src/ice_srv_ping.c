#include "ice_srv_ping.h"
#include <string.h>

#define ICE_STR_HELPER(x) #x
#define ICE_STR(x)        ICE_STR_HELPER(x)

#ifndef ICE_VERSION_MAJOR
#define ICE_VERSION_MAJOR 0
#endif // ICE_VERSION_MAJOR

#ifndef ICE_VERSION_MINOR
#define ICE_VERSION_MINOR 0
#endif // ICE_VERSION_MAJOR

#ifndef ICE_VERSION_PATCH
#define ICE_VERSION_PATCH 1
#endif // ICE_VERSION_PATCH

#define PING_STRING "ICE-Bootloader v" ICE_STR(ICE_VERSION_MAJOR) "." \
                        ICE_STR(ICE_VERSION_MINOR) "." \
                        ICE_STR(ICE_VERSION_PATCH)

static const char ping[] = PING_STRING;

void ice_srv_ping(uint8_t *resp_buf, uint16_t *resp_len) {
    uint16_t data_size = sizeof(ping);
    memcpy(resp_buf, ping, data_size);
    *resp_len = data_size;
}