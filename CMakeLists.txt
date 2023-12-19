cmake_minimum_required(VERSION 3.16)

project(lvglsim)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(SDL_TEST_ENABLED_BY_DEFAULT OFF)
set(LV_CONF_INCLUDE_SIMPLE ON CACHE BOOL "")

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_subdirectory(dependencies/lvgl)
add_subdirectory(dependencies/sdl2)
add_subdirectory(src)
