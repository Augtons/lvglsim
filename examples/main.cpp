#include "lvglsim_interface.h"
#include "lvgl.h"
#include "lv_examples.h"

lvglsim_config on_lvglsim_config() {
    lvglsim_config cfg = {0};
    cfg.width = 800;
    cfg.height = 480;
    cfg.title = "测试";

    return cfg;
}

int app_main(lv_disp_t *disp) {
    lvglsim_init();
    
    lv_example_keyboard_1();

    auto slider = lv_slider_create(lv_disp_get_scr_act(disp));
    lv_obj_set_align(slider, LV_ALIGN_TOP_MID);
    lv_obj_set_pos(slider, 0, 100);

    return 0;
}
