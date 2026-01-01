add_library(common STATIC
    src/ice_ring_buffer.c
    src/ice_cobs.c
)

target_include_directories(
    common PUBLIC include
)