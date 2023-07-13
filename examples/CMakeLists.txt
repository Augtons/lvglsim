cmake_minimum_required(VERSION 3.16)
project(lvglsim_example)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Fill in your lvglsim repository path here.
add_subdirectory(".." lvglsim)

add_executable(lvglsim_example main.cpp)
target_link_libraries(lvglsim_example PUBLIC lvglsim)
