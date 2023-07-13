# lvglsim

其他语言：[English](README.md)

基于 SDL2 的跨平台 LVGL 模拟器。

## 目录

- [用法](#用法)
- [贡献](#贡献)
- [许可证](#许可证)
- [致谢](#致谢)

## 用法

要使用这个模拟器，你需要做以下几步：

1. 克隆这个仓库：`git clone https://github.com/Augtons/lvglsim.git`
2. 创建一个新的 cmake 项目并设置标准为 C++17
3. 将这个仓库作为一个子目录添加：`add_subdirectory(lvglsim)`
4. 为你的项目添加一个可执行文件：`add_executable(my_project main.cpp)`
5. 将 lvglsim 库链接到你的可执行文件：`target_link_libraries(my_project lvglsim)`

这就是 cmake 部分的内容。

这里是一个 CMakeLists.txt 文件的示例，你可以使用：

```cmake
cmake_minimum_required(VERSION 3.10)
project(my_project)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lvglsim)

add_executable(my_project main.cpp)

target_link_libraries(my_project lvglsim)
```

然后你需要编写一些示例代码，例如：

```c
#include "lvglsim_interface.h"
#include "lvgl.h"
#include "lv_examples.h"

lvglsim_config on_lvglsim_config() {
    return { 800, 480, "测试"};
}

extern "C" int app_main(lv_disp_t *disp) {
    lvglsim_init();
    lv_example_keyboard_1();
    return 0;
}
```

这段代码将创建一个模拟器窗口，其中有一个来自 LVGL 的键盘示例。

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
