# lvglsim

其他语言：[English](README.md)

基于 SDL2 的跨平台 LVGL 模拟器。支持多窗口。

## 目录

- [安装](#安装)
- [用法](#用法)
- [贡献](#贡献)
- [许可证](#许可证)
- [致谢](#致谢)

## 安装

不同于一般的的 LVGL 模拟器，这个模拟器是以 **静态库 (Static Library)** 来分发的。
你需要先安装这个库，然后创建一个新的项目或者使用下面的示例项目来使用这个库。

目的是为了**避免重复编译**和**使编译的产品可以随意分发**。

1. **克隆本仓库：** **务必添加** `--recursive` **标志！！！**
   ```shell
   git clone --recursive https://github.com/Augtons/lvglsim.git
   ```
2. **编译并安装本仓库：**

   把`<Your Install Path>`替换成你的**安装路径**！
   
   将使用默认编译器编译。
   若要切换编译器，请自行查阅 cmake 有关资料。
   ```shell
   cmake -B ./build -DCMAKE_INSTALL_PREFIX="<Your Install Path>"
   cmake --build ./build
   cmake --install ./build --component lvglsim
   ```

> **注意事项:**
> 
> 使用cmake安装这个库时，被安装的仅是**基于构建树的导出文件** (`lvglsimConfig.cmake`)。
> 所以你需要确保 cmake 编译中间文件夹**不被移动** (例如上文中的 `./build` 文件夹)。
>
> 这样做的好处是避免了繁琐的依赖管理
>
> **使用系统 SDL2 库:**
> 
> 若构建此项目时，CMake 成功使用 `find_package` 找到 SDL2 库，那么就会**直接使用而不是从源码编译**。
> 
> 如果你想使用已被安装到的 SDL2 库，或者你正在交叉编译本项目。
> 你可以在加载项目时指定 `-DCMAKE_PREFIX_PATH=<路径>`，将包含 `SDL2/SDL2Config.cmake` 的**父路径**添加进去
>
> 如果想**强制编译源码中的 SDL 库**，您可在加载项目时指定 `-DUSE_SDL_FROM_SRC=ON`
> 
> **卸载:**
> 
> 如果你想卸载这个库，只需要删除导出文件 (export file) 以及删除cmake编译中间文件夹

## 用法

要使用这个模拟器，你需要做以下几步，或者使用 **[<u>示例工程</u>](/examples)：**

1. 创建一个新的 CMake C/C++ 项目。
2. 找到这个已安装的库：
   ```cmake
   # 使用 `find_package`或者 `include` 你安装得到的导出文件.
   
   # 以下方式二选一
   # include(lvglsimConfig.cmake)
   find_package(lvglsim REQUIRED)
   ```
3. 为你的项目添加一个可执行文件：
   ```cmake
   add_executable(my_project main.cpp)
   ```
4. 把lvglsim库链接到你的可执行文件：
   ```cmake
   target_link_libraries(my_project lvglsim::lvglsim)
   ```

这就是cmake部分的内容。

下面是一个可以使用的CMakeLists.txt文件的例子：

```cmake
cmake_minimum_required(VERSION 3.16)
project(lvglsim_example)

# 如果你需要添加`CMAKE_PREFIX_PATH`
# list(APPEND CMAKE_PREFIX_PATH "<Your Path>")

# 使用 `find_package` 或者 `include` 你安装的 export 文件
# include(lvglsimConfig.cmake)
find_package(lvglsim REQUIRED)

add_executable(lvglsim_example main.cpp)
target_link_libraries(lvglsim_example PUBLIC lvglsim::lvglsim)
```

然后你需要写一些示例代码，比如：

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

// 不要使用`main()`函数，因为SDL2封装了它。
// 请使用`int app_main()`
int app_main(int argc, char *argv[]) {
    // 1. 初始化 lvglsim
    window_config_t cfg;
    cfg.width = 800;
    cfg.height = 480;
    cfg.title = "MainWindow";

    // 2. 创建窗口
    uint32_t window_id = lvglsim_new_window(&cfg);
    lv_disp_t* disp = lvglsim_get_disp(window_id);

    // 3. LVGL官方的示例可以直接使用
    lv_example_keyboard_1();

    // 4. 像这样绘制你自己的UI！
    lv_obj_t* slider = lv_slider_create(lv_disp_get_scr_act(disp));
    lv_obj_set_align(slider, LV_ALIGN_TOP_MID);
    lv_obj_set_pos(slider, 0, 100);

    // 5. 多窗口
    lv_obj_t* button = lv_btn_create(lv_disp_get_scr_act(disp));
    lv_obj_set_align(button, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_pos(button, 0, -300);
    lv_label_set_text(lv_label_create(button), "Create Window");
    lv_obj_add_event_cb(button, on_click, LV_EVENT_CLICKED, NULL);

    // 6. 返回 0 表示初始化成功！
    //    然后lvglsim就会启动，否则程序会以这个错误码退出。
    return 0;
}
```

这段代码将创建一个模拟器窗口，其中有一个来自LVGL的键盘示例。

## 贡献

如果你想为这个项目做出贡献，请遵循以下准则：

- Fork 这个仓库并为你的功能或修复创建一个新的分支
- 确保你的代码格式良好并遵循这个项目的编码风格
- 编写清晰简洁的提交信息和拉取请求描述
- 在提交拉取请求之前测试你的代码
- 尊重行为准则并对其他贡献者保持礼貌

## 许可证

这个项目使用 `Apache-2.0 license` 许可证授权 - 请查看 [LICENSE](LICENSE) 文件了解详情。

## 致谢

这个项目基于以下库和项目：

- [LVGL](https://github.com/lvgl/lvgl) - 一个强大且易于使用的嵌入式 GUI 库
- [SDL2](https://www.libsdl.org/) - 一个用于多媒体和输入的跨平台开发库
