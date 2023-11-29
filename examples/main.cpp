#include "lvglsim.h"
#include "lv_examples.h"

// DON'T USE `main()` function, because SDL2 wrapped it.
// Please use `int app_main()`
int app_main() {
    // 1. Initialize the lvglsim
    lvglsim_config_t cfg;
    cfg.width = 800;
    cfg.height = 480;
    cfg.title = "测试";

    lvglsim_init(&cfg);

    // 2. Get the main display object.
    lv_disp_t *disp = get_main_disp();

    // 3. The official LVGL examples can be used directly
    lv_example_keyboard_1();

    // 4. Draw your own UI like this!
    auto slider = lv_slider_create(lv_disp_get_scr_act(disp));
    lv_obj_set_align(slider, LV_ALIGN_TOP_MID);
    lv_obj_set_pos(slider, 0, 100);

    // 5. A return of 0 means that the initialization was successful!
    //    Then lvglsim will be started, otherwise the program will exit with this error code.
    return 0;
}
