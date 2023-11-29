cmake_minimum_required(VERSION 3.16)
project(lvglsim_example)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

find_package(lvglsim REQUIRED)

add_executable(lvglsim_example main.cpp)
target_link_libraries(lvglsim_example PUBLIC lvglsim::lvglsim)
