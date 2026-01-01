#include <stdint.h>
typedef enum {
    ICE_PQ_FULL,
    ICE_PQ_EMPTY,
    ICE_PQ_OK
} ice_pq_status_t;

typedef struct {
    volatile uint16_t head;
    volatile uint16_t tail;
    uint16_t size_mask;
 
} ice_packet_queue_t;


ice_pq_status_t ice_packet_queue_init(ice_packet_queue_t* q, );
ice_pq_status_t ice_packet_queue_push(ice_packet_queue_t* q, );
ice_pq_status_t ice_packet_queue_pop(ice_packet_queue_t* q, );