#include <stdint.h>
#include "ice_delay.h"
#include "ice_ring_buffer.h"
#include "ice_transport.h"
#include "usbd_cdc_if.h"

extern void CDC_RegisterRxCallback(void (*cb)(uint8_t*, uint32_t));

static uint8_t buffer[2048];
static ICE_ring_buffer_t usb_ring_buffer;


static int32_t usb_write(ICE_TRANSPORT_t* self, const uint8_t *data, uint16_t len){
    uint8_t status;
    uint32_t start = ICE_get_tick();
    do {
        status = CDC_Transmit_FS((uint8_t*) data, len);

        if ((ICE_get_tick() - start) > 10) return -1;
    } while (status == USBD_BUSY);

    return (status == USBD_OK) ? 0 : -1;
}

static void usb_rx_handler(uint8_t* Buf, uint32_t Len){
    ICE_ring_buffer_put_many(&usb_ring_buffer, Buf, Len);
}

static int32_t usb_read(ICE_TRANSPORT_t* self, uint8_t *data, uint16_t len){
    int32_t bytes_read = 0;
    while (bytes_read < len){
        if (ICE_ring_buffer_pop(&usb_ring_buffer, &data[bytes_read]) == ICE_RB_OK){
            bytes_read++;
        } else {
            break;
        }
    }
    return bytes_read;
}

static const ICE_TRANSPORT_ops_t usb_ops = {
    .write = usb_write,
    .read = usb_read,
};


void ICE_TRANSPORT_usb_init(ICE_TRANSPORT_t* transport){
    ICE_ring_buffer_init(&usb_ring_buffer, buffer, sizeof(buffer));
    CDC_RegisterRxCallback(usb_rx_handler);
    transport->ops = &usb_ops;
    transport->handle = NULL;
}