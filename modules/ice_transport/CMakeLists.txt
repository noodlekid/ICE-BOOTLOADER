add_library(ice_transport
    src/ice_transport_usb.c
)

target_include_directories(ice_transport PUBLIC
    include
)

target_link_libraries(ice_transport PUBLIC
    common
    STM32_Drivers
)
