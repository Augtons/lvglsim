cmake_minimum_required(VERSION 3.16)

project(lvglsim)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(SDL_TEST_ENABLED_BY_DEFAULT OFF)
set(LV_CONF_INCLUDE_SIMPLE ON CACHE BOOL "")

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_subdirectory(dependencies/lvgl)

find_package(SDL2 CONFIG COMPONENTS SDL2 SDL2main SDL2-static)

if(USE_SDL_FROM_SRC)
  message(STATUS "Using the SDL2 from the source code.")
  add_subdirectory(dependencies/sdl2)
elseif(NOT SDL2_FOUND)
  message(WARNING "SDL2 notfound in your system! Using the SDL2 from the source code.")
  add_subdirectory(dependencies/sdl2)
else()
  message(STATUS "Installed SDL2 has been found in your system. Using the SDL2 from your system.")
endif()

add_subdirectory(src)