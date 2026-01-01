add_library(ice_services
    src/ice_srv_ping.c
)

target_include_directories(ice_services PUBLIC
    include
)

target_link_libraries(ice_services PUBLIC
    ice_command_handler
)
