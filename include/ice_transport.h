#ifndef ICE_TRANSPORT_H
#define ICE_TRANSPORT_H

#include <stdint.h>

typedef enum {
    ICE_TRANSPORT_OK = 0,
    ICE_TRANSPORT_ERROR,
} ICE_TRANSPORT_status_t;

struct ICE_TRASPORT_s;

typedef struct {
    int32_t (*write)(struct ICE_TRASPORT_s *self, const uint8_t *data, uint16_t len);
    int32_t (*read)(struct ICE_TRASPORT_s *self, const uint8_t *data, uint16_t len);
} ICE_TRANSPORT_ops_t;

#endif // ICE_TRANSPORT_H