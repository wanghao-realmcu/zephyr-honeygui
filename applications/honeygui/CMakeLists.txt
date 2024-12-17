cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(honeygui)

add_subdirectory(driver)

target_sources(app PRIVATE app/main.c)

target_link_libraries(app PRIVATE reaktek_wearable_library)
target_link_libraries(app PRIVATE reaktek_display_driver_library)

target_link_options(app
    PUBLIC
        -Wl,--wrap,mac_EnterCritical_rom
        -Wl,--wrap,mac_ExitCritical_rom
        -Wl,--wrap,mac_RstRF_rom
        -Wl,--wrap,otPlatReset
)
