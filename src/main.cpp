#include "SDL2/SDL.h"
#include "DisplayDriver.h"
#include "DynamicLibrary.h"
#include "lvglsim.h"
#include "lv_examples.h"

auto& driver = DisplayDriver::instance();

void lvglsim_init(const lvglsim_config_t *config) {
    lv_init();

    driver.initWindow(config->title, config->width, config->height);
    driver.initDisplayDriver();
    driver.initInputDriver();
}

lv_disp_t *get_main_disp() {
    return driver.getMainDisplayObj();
}

int main(int argc, char *argv[]) {
    int ret = app_main();
    if (ret != 0) {
        return ret;
    }
    driver.mainLoop();

    return 0;
}
