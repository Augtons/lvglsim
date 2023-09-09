#ifndef LVGLSIM_DISPLAYDRIVER_H
#define LVGLSIM_DISPLAYDRIVER_H

#include <iostream>
#include <stdexcept>
#include <memory>
#include <mutex>
#include "UniqueNativeObject.hpp"
#include "lvgl.h"
#include "SDL2/SDL.h"

template<typename T>
using unique_cobj = std::unique_ptr<T, void(*)(T*)>;

class DisplayDriver {
public:

    struct MouseInputStatus {
        bool mouse_pressed = false;
        int mouse_x = 0;
        int mouse_y = 0;
    };

    static DisplayDriver& instance();

    DisplayDriver(const DisplayDriver&) = delete;
    DisplayDriver& operator= (const DisplayDriver&) = delete;

    ~DisplayDriver();

    void initWindow(const char* title, int width, int height);
    void initDisplayDriver();
    void initInputDriver();

    void flushToWindow(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p);
    lv_disp_t *getMainDisplayObj() { return main_disp; }

    void mainLoop();
private:
    bool _has_inited_window = false;
    bool _has_display_inited = false;
    bool _has_indev_inited = false;

    int width = 0;
    int height = 0;

    lv_disp_draw_buf_t disp_buf = {};
    lv_disp_drv_t disp_drv = {};
    lv_disp_t *main_disp = nullptr;
    lv_color_t *buf = nullptr;
    lv_color_t *buf_1 = nullptr;
    lv_color_t *buf_2 = nullptr;

    lv_indev_drv_t mouse_indev_drv = {};
    lv_indev_t *mouse_indev;

    std::mutex mouseInputStatusMutex;
    MouseInputStatus mouseInputStatus;

    UniqueNativeObject<SDL_Window, SDL_DestroyWindow> main_window;
    UniqueNativeObject<SDL_Renderer, SDL_DestroyRenderer> main_renderer;
    UniqueNativeObject<SDL_Texture, SDL_DestroyTexture> main_texture;

    DisplayDriver() = default;

    void mousePress(int x, int y);
    void mouseRelease();

    inline lv_color_t& point_at(int x, int y) {
        return buf[y * width + x];
    }

    inline MouseInputStatus inputStatus() {
        mouseInputStatusMutex.lock();
        auto ret = mouseInputStatus;
        mouseInputStatusMutex.unlock();
        return ret;
    }
};

#endif //LVGLSIM_DISPLAYDRIVER_H
