target_include_directories(lvgl PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(lvgl_examples PUBLIC ${CMAKE_CURRENT_LIST_DIR})

file(GLOB_RECURSE srcs LIST_DIRECTORIES false "*.cpp" "*.c")

add_library(lvglsim STATIC ${srcs})

target_include_directories(lvglsim PUBLIC include)
target_include_directories(lvglsim PRIVATE include_private)
target_include_directories(lvglsim PRIVATE hal)

target_link_libraries(lvglsim PRIVATE SDL2::SDL2main SDL2::SDL2-static)
target_link_libraries(lvglsim PUBLIC lvgl::lvgl lvgl::examples)

