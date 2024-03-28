cmake_minimum_required(VERSION 3.16)
project(lvglsim_example)

# 1. Specifying C C++ Standard like this. (Optional)
# set(CMAKE_C_STANDARD 11)
# set(CMAKE_CXX_STANDARD 17)

# 2. If you need add `CMAKE_PREFIX_PATH` (Optional)
# list(APPEND CMAKE_PREFIX_PATH "<Your Path>")

# 3. Use `find_package` or `include` the export file you had installed.
#    Or just call add_subdirectory with lvglsim source directory.
# include(lvglsimConfig.cmake)
# find_package(lvglsim REQUIRED)
add_subdirectory(".." lvglsim)

add_executable(lvglsim_example main.c)
target_link_libraries(lvglsim_example PUBLIC lvglsim::lvglsim)
