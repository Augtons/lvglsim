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
