target_include_directories(lvgl PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(lvgl_examples PUBLIC ${CMAKE_CURRENT_LIST_DIR})

file(GLOB_RECURSE srcs "*.cpp" "*.c")

add_library(lvglsim STATIC ${srcs})
add_library(lvglsim::lvglsim ALIAS lvglsim)

target_include_directories(lvglsim PUBLIC include)
target_include_directories(lvglsim PRIVATE include_private hal)

target_link_libraries(lvglsim PRIVATE SDL2::SDL2main SDL2::SDL2-static)
target_link_libraries(lvglsim PUBLIC lvgl::lvgl lvgl::examples)

export(TARGETS lvglsim SDL2main SDL2-static sdl-build-options lvgl lvgl_examples
  NAMESPACE lvglsim::
  FILE lvglsimConfig.cmake
)

install(CODE
  "file(INSTALL ${CMAKE_CURRENT_BINARY_DIR}/lvglsimConfig.cmake DESTINATION \"${CMAKE_INSTALL_PREFIX}/cmake\")"
  COMPONENT lvglsim
)

