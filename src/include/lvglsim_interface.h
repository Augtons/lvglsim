#ifndef LVGLSIM_LVGLSIM_INTERFACE_H
#define LVGLSIM_LVGLSIM_INTERFACE_H

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

struct lvglsim_config {
    lv_coord_t width;
    lv_coord_t height;

    const char *title;
};


lvglsim_config on_lvglsim_config();

void lvglsim_init();

int app_main(lv_disp_t *disp);

#ifdef __cplusplus
}
#endif

#endif //LVGLSIM_LVGLSIM_INTERFACE_H
