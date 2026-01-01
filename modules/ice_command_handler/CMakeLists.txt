add_library(ice_command_handler
    src/ice_command_handler.c
)

target_include_directories(ice_command_handler PUBLIC
    include
)

target_link_libraries(ice_command_handler PUBLIC
    ice_comms_engine
    ice_services
)
