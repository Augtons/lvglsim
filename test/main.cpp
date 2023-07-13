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