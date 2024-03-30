# lvglsim

Other Languages: [简体中文](README_zh_CN.md)

A Cross-Platform Simulator Library for LVGL using SDL2. Supports Multi-Window.

## Contents

- [Install](#install)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgements](#acknowledgements)

## Install

Unlike the normal LVGL simulator, this simulator is distributed as **A Static Library**.
You need to install this library first, and then create a new project or use the sample project below to use this library.

The purpose is to **avoid repeated compilation** and **make the compiled product distributable** at will.

1. **Clone this repository**: **Be sure to add** `--recursive` **flag !!!**
   ```shell
   git clone --recursive https://github.com/Augtons/lvglsim.git
   ```
2. **Build and install this repository:**
   
   Replace `<Your Install Path>` with your **path to install** this library!
   
   Will use the default compiler.
   To switch compilers, please refer to the cmake documentation.
   
   ```shell
   cmake -B ./build -DCMAKE_INSTALL_PREFIX="<Your Install Path>"
   cmake --build ./build
   cmake --install ./build --component lvglsim
   ```

> **Note:**
>
> When installing this library with cmake, what will be installed is **only the export file** (`lvglsimConfig.cmake`) **based on the build tree**.
> So you need to ensure that the cmake compilation intermediate folder **CAN'T be moved** (e.g. the `./build` folder) .
> 
> The benefit of this is to avoid cumbersome dependency management
>
> **Using the System SDL2 Library:**
> 
> If CMake successfully locates the SDL2 library using `find_package` during the project build,
> it will **directly use it instead of compiling from source**.
> 
> If you want to use an already installed SDL2 library or if you are cross-compiling this project,
> you can specify `-DCMAKE_PREFIX_PATH=<path>` when loading the project,
> for adding the **parent path** containing `SDL2/SDL2Config.cmake`.
> 
> To **force compilation of the SDL library from the source code**,
> specify `-DUSE_SDL_FROM_SRC=ON` when loading the project
> 
> **Uninstall:**
>
> If you want to uninstall this library, only requires deleting the export file and deleting the cmake compilation intermediate folder

## Usage

To use this simulator, you need to do the following steps or **[<u>Example Project</u>](/examples)**:

1. Create a new CMake C/C++ project.
2. Find this installed library:
   ```cmake
   # Use `find_package` or `include` the export file you had installed.
   
   # Choose one of the following methods
   # include(lvglsimConfig.cmake)
   find_package(lvglsim REQUIRED)
   ```
3. Add an executable for your project:
   ```cmake
   add_executable(my_project main.cpp)
   ```
4. Link the lvglsim library to your executable:
   ```cmake
   target_link_libraries(my_project lvglsim::lvglsim)
   ```

That's it for the cmake part.

Here is an example of a CMakeLists.txt file that you can use:

```cmake
cmake_minimum_required(VERSION 3.16)
project(lvglsim_example)

# If you need add `CMAKE_PREFIX_PATH` (Optional)
# list(APPEND CMAKE_PREFIX_PATH "<Your Path>")

# Use `find_package` or `include` the export file you had installed.
# include(lvglsimConfig.cmake)
find_package(lvglsim REQUIRED)

add_executable(lvglsim_example main.cpp)
target_link_libraries(lvglsim_example PUBLIC lvglsim::lvglsim)
```

Then you need to write some example code, such as:

```c
#include "lvglsim.h"
#include "lv_examples.h"

void on_click(lv_event_t* e) {
    window_config_t cfg;
    cfg.width = 480;
    cfg.height = 320;
    cfg.title = "SubWindow";

    uint32_t window_id = lvglsim_new_window(&cfg);
    lv_disp_t* sub_disp = lvglsim_get_disp(window_id);

    lv_disp_set_default(sub_disp);
    lv_example_keyboard_1();
}

// DON'T USE `main()` function, because SDL2 wrapped it.
// Please use `int app_main()`
int app_main(int argc, char *argv[]) {
    // 1. Initialize the lvglsim
    window_config_t cfg;
    cfg.width = 800;
    cfg.height = 480;
    cfg.title = "MainWindow";

    // 2. Create the main window.
    uint32_t window_id = lvglsim_new_window(&cfg);
    lv_disp_t* disp = lvglsim_get_disp(window_id);

    // 3. The official LVGL examples can be used directly
    lv_example_keyboard_1();

    // 4. Draw your own UI like this!
    lv_obj_t* slider = lv_slider_create(lv_disp_get_scr_act(disp));
    lv_obj_set_align(slider, LV_ALIGN_TOP_MID);
    lv_obj_set_pos(slider, 0, 100);

    // 5. Multi-Window
    lv_obj_t* button = lv_btn_create(lv_disp_get_scr_act(disp));
    lv_obj_set_align(button, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_pos(button, 0, -300);
    lv_label_set_text(lv_label_create(button), "Create Window");
    lv_obj_add_event_cb(button, on_click, LV_EVENT_CLICKED, NULL);

    // 6. A return of 0 means that the initialization was successful!
    //    Then lvglsim will be started, otherwise the program will exit with this error code.
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

This project is licensed under the `Apache-2.0 license` License - see the [LICENSE](LICENSE) file for details.

## Acknowledgements

This project is based on the following libraries and projects:

- [LVGL](https://github.com/lvgl/lvgl) - A powerful and easy-to-use embedded GUI library
- [SDL2](https://www.libsdl.org/) - A cross-platform development library for multimedia and input
