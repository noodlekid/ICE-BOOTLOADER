add_library(ice_flash
    src/ice_flash.c
)

target_include_directories(ice_flash PUBLIC
    include
)

target_link_libraries(ice_flash PUBLIC
    common
    STM32_Drivers
)