#include "SDL2/SDL.h"
#include "DisplayDriver.h"
#include "DynamicLibrary.h"
#include "lvglsim_interface.h"
#include "lv_examples.h"

auto& driver = DisplayDriver::instance();

void lvglsim_init() {
    lv_init();

    auto config = on_lvglsim_config();

    driver.initWindow(config.title, config.width, config.height);
    driver.initDisplayDriver();
    driver.initInputDriver();
}

int main(int argc, char *argv[]) {
    app_main(driver.getMainDisplayObj());
    driver.mainLoop();

    return 0;
}
