add_library(ice_comms_engine
    src/ice_packetizer.c
    src/ice_packet_pool.c
    src/ice_comms_engine.c
    src/ice_comms_frame_queue.c
)

target_include_directories(ice_comms_engine PUBLIC 
    include
)

target_link_libraries(ice_comms_engine PUBLIC
    common
    ice_transport
)