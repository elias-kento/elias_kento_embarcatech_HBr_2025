cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(pico_led_modular C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(main
    app/main.c
    drivers/led_embutido.c
    hal/hal_led.c
)

target_include_directories(main PRIVATE include)
target_link_libraries(main pico_stdlib pico_cyw43_arch)

pico_add_extra_outputs(main)
