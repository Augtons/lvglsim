# lvglsim

Other Languages: [简体中文](README_zh_CN.md)

A Cross-Platform Simulator for LVGL using SDL2.

## Contents

- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgements](#acknowledgements)

## Usage

To use this simulator, you need to do the following steps:

1. Clone this repository: `git clone https://github.com/Augtons/lvglsim.git`
2. Create a new cmake project and set the standard to C++17
3. Add this repository as a subdirectory: `add_subdirectory(lvglsim)`
4. Add an executable for your project: `add_executable(my_project main.cpp)`
5. Link the lvglsim library to your executable: `target_link_libraries(my_project lvglsim)`

That's it for the cmake part.

Here is an example of a CMakeLists.txt file that you can use:

```cmake
cmake_minimum_required(VERSION 3.10)
project(my_project)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lvglsim)

add_executable(my_project main.cpp)

target_link_libraries(my_project lvglsim)
```

Then you need to write some example code, such as:

```c
#include "lvglsim_interface.h"
#include "lvgl.h"
#include "lv_examples.h"

lvglsim_config on_lvglsim_config() {
    return { 800, 480, "Test"};
}

extern "C" int app_main(lv_disp_t *disp) {
    lvglsim_init();
    lv_example_keyboard_1();
    return 0;
}
```

This code will create a simulator window with a keyboard example from LVGL.

## Contributing

If you want to contribute to this project, please follow these guidelines:

- Fork this repository and create a new branch for your feature or bugfix
- Make sure your code is well-formatted and follows the coding style of this project
- Write clear and concise commit messages and pull request descriptions
- Test your code before submitting a pull request
- Respect the code of conduct and be polite to other contributors

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgements

This project is based on the following libraries and projects:

- [LVGL](https://github.com/lvgl/lvgl) - A powerful and easy-to-use embedded GUI library
- [SDL2](https://www.libsdl.org/) - A cross-platform development library for multimedia and input
