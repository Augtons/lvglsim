target_include_directories(lvgl PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(lvgl_examples PUBLIC ${CMAKE_CURRENT_LIST_DIR})

file(GLOB_RECURSE srcs "*.cpp" "*.c")

add_library(lvglsim STATIC ${srcs})
add_library(lvglsim::lvglsim ALIAS lvglsim)

target_include_directories(lvglsim PUBLIC include)
target_include_directories(lvglsim PRIVATE include_private hal)

target_link_libraries(lvglsim PUBLIC lvgl::lvgl lvgl::examples)

if((NOT SDL2_FOUND) OR (SDL2_SDL2-static_FOUND))
  message(STATUS "SDL2 static links will be used.")
  set(LVGLSIM_SDL_DEPENDIES SDL2::SDL2main SDL2::SDL2-static)
elseif(SDL2_SDL2_FOUND)
  message(STATUS "SDL2 dynamic links will be used.")
  set(LVGLSIM_SDL_DEPENDIES SDL2::SDL2main SDL2::SDL2)
endif()

target_link_libraries(lvglsim PRIVATE ${LVGLSIM_SDL_DEPENDIES})

# Export And Install
set(LVGLSIM_EXPORT_TARGETS lvglsim lvgl lvgl_examples)

if(NOT SDL2_FOUND)
  list(APPEND LVGLSIM_EXPORT_TARGETS SDL2main SDL2-static sdl-build-options)
endif()

export(TARGETS ${LVGLSIM_EXPORT_TARGETS}
  NAMESPACE lvglsim::
  FILE lvglsimConfig.cmake
)

install(CODE
  "file(INSTALL ${CMAKE_CURRENT_BINARY_DIR}/lvglsimConfig.cmake DESTINATION \"${CMAKE_INSTALL_PREFIX}/cmake\")"
  COMPONENT lvglsim
)
