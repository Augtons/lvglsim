#ifndef LVGLSIM_LVGLSIM_H
#define LVGLSIM_LVGLSIM_H

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct lvglsim_config {
    lv_coord_t width;
    lv_coord_t height;

    const char *title;
} lvglsim_config_t;

void lvglsim_init(const lvglsim_config_t *config);

lv_disp_t *get_main_disp();

int app_main();

#ifdef __cplusplus
}
#endif

#endif //LVGLSIM_LVGLSIM_H
