# lvglsim

其他语言：[English](README.md)

基于 SDL2 的跨平台 LVGL 模拟器。

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

1. **克隆本仓库：**
   ```shell
   git clone --recursive https://github.com/Augtons/lvglsim.git
   ```
2. **编译并安装本仓库：**

   把`<Your Install Path>`替换成你的安装路径！
   ```shell
   cmake -B ./build -DCMAKE_INSTALL_PREFIX="<Your Install Path>"
   cmake --build ./build
   cmake --install ./build --component lvglsim
   ```

> **注意事项:**
> 
> 使用cmake安装这个库时，安装的是**基于构建树的导出文件 (export file)**。
> 所以你需要确保cmake编译中间文件夹**不被移动**。
>
> 这样做的好处是避免了繁琐的依赖管理
>
> 如果你想卸载这个库，只需要删除导出文件 (export file) 以及删除cmake编译中间文件夹

## 用法

要使用这个模拟器，你需要做以下几步：

1. 创建一个新的 CMake C/C++ 项目。
2. 找到这个已安装的库：
   ```cmake
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

add_executable(lvglsim_example main.cpp)
target_link_libraries(lvglsim_example PUBLIC lvglsim::lvglsim)
```

然后你需要写一些示例代码，比如：

```c
#include "lvglsim.h"
#include "lv_examples.h"

// 不要使用`main()`函数，因为SDL2封装了它。
// 请使用`int app_main()`
int app_main() {
    // 1. 初始化lvglsim
    lvglsim_config_t cfg;
    cfg.width = 800;
    cfg.height = 480;
    cfg.title = "测试";

    lvglsim_init(&cfg);

    // 2. 获取主显示对象。
    lv_disp_t *disp = get_main_disp();

    // 3. LVGL官方的示例可以直接使用
    lv_example_keyboard_1();

    // 4. 像这样绘制你自己的UI！
    auto slider = lv_slider_create(lv_disp_get_scr_act(disp));
    lv_obj_set_align(slider, LV_ALIGN_TOP_MID);
    lv_obj_set_pos(slider, 0, 100);

    // 5. 返回0表示初始化成功！
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
