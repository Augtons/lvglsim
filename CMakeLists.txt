cmake_minimum_required(VERSION 3.16)

project(lvglsim)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if(CMAKE_CURRENT_LIST_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "Current directory '${CMAKE_CURRENT_LIST_DIR}' is not buildable. "
    "Change directories to one of the example projects in '${CMAKE_CURRENT_LIST_DIR}/examples' and try "
    "again.\n"
    "当前目录 '${CMAKE_CURRENT_LIST_DIR}' 不可构建。 "
    "请切换到 '${CMAKE_CURRENT_LIST_DIR}/examples' 中的一个示例项目目录，然后重试。")
endif()

set(SDL_TEST_ENABLED_BY_DEFAULT OFF)
set(LV_CONF_INCLUDE_SIMPLE ON CACHE BOOL "")

add_subdirectory(dependencies/lvgl)
add_subdirectory(dependencies/sdl2)
add_subdirectory(src)
