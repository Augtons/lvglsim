#ifndef LVGLSIM_LVGLSIM_H
#define LVGLSIM_LVGLSIM_H

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct window_config {
    lv_coord_t width;
    lv_coord_t height;
    const char *title;
} window_config_t;

uint32_t lvglsim_new_window(const window_config_t *config);
lv_disp_t* lvglsim_get_disp(uint32_t windowId);

void lvglsim_lock();
void lvglsim_unlock();
bool lvglsim_try_lock();

int app_main(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif

#endif //LVGLSIM_LVGLSIM_H
