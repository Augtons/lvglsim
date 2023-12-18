#include <thread>
#include "Timer.hpp"
#include "DisplayDriver.h"
#include "lvgl.h"

DisplayDriver& DisplayDriver::instance() {
    static DisplayDriver ins;
    return ins;
}

DisplayDriver::~DisplayDriver() {
    if (mouse_indev != nullptr) {
        lv_indev_delete(mouse_indev);
        mouse_indev = nullptr;
    }
    if (main_disp != nullptr) {
        lv_disp_remove(main_disp);
        main_disp = nullptr;
    }
    delete[] buf;
    delete[] buf_1;
    delete[] buf_2;
    main_window = nullptr;
    main_renderer = nullptr;
    main_texture = nullptr;
    SDL_Quit();
}

void DisplayDriver::initWindow(const char* title, int width, int height) {
    if (_has_inited_window) {
        return;
    }
    this->width = width;
    this->height = height;

    int ret = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    if (ret < 0) {
        throw std::runtime_error(SDL_GetError());
    }

    auto window = UniqueNativeObject<SDL_Window, SDL_DestroyWindow>(
        SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN)
    );
    if (window == nullptr) {
        throw std::runtime_error(SDL_GetError());
    }

    auto renderer = UniqueNativeObject<SDL_Renderer, SDL_DestroyRenderer>(
        SDL_CreateRenderer((SDL_Window*)window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)
    );
    if (renderer == nullptr) {
        throw std::runtime_error(SDL_GetError());
    }
    main_window = std::move(window);
    main_renderer = std::move(renderer);
    _has_inited_window = true;
}

void DisplayDriver::initDisplayDriver() {
    if (_has_display_inited) {
        return;
    }
    if (!_has_inited_window) {
        throw std::runtime_error("Please call initWindow() first");
    }
    auto texture = UniqueNativeObject<SDL_Texture, SDL_DestroyTexture>(
        SDL_CreateTexture((SDL_Renderer*)main_renderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_TARGET, width, height)
    );
    if (texture == nullptr) {
        throw std::runtime_error(SDL_GetError());
    }
    buf = new lv_color_t[height * width];
    buf_1 = new lv_color_t[width * 20 * sizeof(lv_color_t)];
    buf_2 = new lv_color_t[width * 20 * sizeof(lv_color_t)];

    lv_disp_draw_buf_init(&disp_buf, buf_1, buf_2, width * 20 * sizeof(lv_color_t));
    lv_disp_drv_init(&disp_drv);

    disp_drv.draw_buf = &disp_buf;
    disp_drv.hor_res = (lv_coord_t)width;
    disp_drv.ver_res = (lv_coord_t)height;
    disp_drv.user_data = (void*)this;

    disp_drv.flush_cb = [](lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p) {
        auto self = (DisplayDriver*)drv->user_data;
        self->flushToWindow(drv, area, color_p);
    };

    main_disp = lv_disp_drv_register(&disp_drv);
    main_texture = std::move(texture);
}

void DisplayDriver::initInputDriver() {
    if (_has_indev_inited) {
        return;
    }
    if (!_has_inited_window) {
        throw std::runtime_error("Please call initWindow() first");
    }

    lv_indev_drv_init(&mouse_indev_drv);
    mouse_indev_drv.type = LV_INDEV_TYPE_POINTER;
    mouse_indev_drv.user_data = (void*)this;
    mouse_indev_drv.read_cb = [](lv_indev_drv_t * drv, lv_indev_data_t * data) {
        auto self = (DisplayDriver*)drv->user_data;
        auto mouse = self->getMouseInputStatus();
        data->state = mouse.mouse_pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
        data->point.x = (lv_coord_t)mouse.mouse_x;
        data->point.y = (lv_coord_t)mouse.mouse_y;
    };
    mouse_indev = lv_indev_drv_register(&mouse_indev_drv);

    lv_indev_drv_init(&keyboard_indev_drv);
    keyboard_indev_drv.type = LV_INDEV_TYPE_KEYPAD;
    keyboard_indev_drv.user_data = (void*)this;
    keyboard_indev_drv.read_cb = [](lv_indev_drv_t * drv, lv_indev_data_t * data) {
        auto self = (DisplayDriver*)drv->user_data;
        auto key = self->fetchKeyboardCode();
        if (key) {
            data->state = LV_INDEV_STATE_PRESSED;
            data->key = key.value();
            // printf("LVGL Key Down: %c", data->key);
        } else {
            data->state = LV_INDEV_STATE_RELEASED;
        }
    };
    keyboard_indev = lv_indev_drv_register(&keyboard_indev_drv);
}

void DisplayDriver::flushToWindow(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p) {
    for(int y = 0; y <= area->y2 - area->y1; y++) {
        for(int x = 0; x <= area->x2 - area->x1; x++) {
            point_at(x, y).full = color_p->full;
            color_p++;
        }
    }

    SDL_Rect rect = {area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1};
    SDL_UpdateTexture((SDL_Texture*)main_texture, &rect, buf, width * (int)sizeof(lv_color_t));

    if (lv_disp_flush_is_last(drv)) {
        SDL_RenderCopy((SDL_Renderer*)main_renderer, (SDL_Texture*)main_texture, nullptr, nullptr);
        SDL_RenderPresent((SDL_Renderer*)main_renderer);
    }

    lv_disp_flush_ready(drv);
}

void DisplayDriver::mousePress(int x, int y) {
    mouseInputStatusMutex.lock();
    mouseInputStatus.mouse_pressed = true;
    mouseInputStatus.mouse_x = x;
    mouseInputStatus.mouse_y = y;
    mouseInputStatusMutex.unlock();
}

void DisplayDriver::mouseRelease() {
    mouseInputStatusMutex.lock();
    mouseInputStatus.mouse_pressed = false;
    mouseInputStatusMutex.unlock();
}

void DisplayDriver::keyDown(SDL_Keycode key) {
    keyboardInputStatusMutex.lock();
    keyInputStatusQueue.push(key);
    keyboardInputStatusMutex.unlock();
}

void DisplayDriver::mainLoop() {
    if (!_has_inited_window) {
        throw std::runtime_error("Please call lvglsim_init() first.");
    }

    auto timer = SysTimer(10, [](uint32_t interval, void*) -> uint32_t {
        lv_tick_inc(10);
        return interval;
    });

    auto eventThread = std::thread([] {
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            lv_task_handler();
        }
    });
    eventThread.detach();

    SDL_Event event;
    while (true) {
        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
                case SDL_QUIT: {
                    return;
                }
                case SDL_MOUSEBUTTONUP: {
                    mouseRelease();
                    break;
                }
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEMOTION: {
                    int x, y;
                    if (SDL_GetMouseState(&x, &y) & 0x01) {
                        mousePress(x, y);
                    } else {
                        mouseRelease();
                    }
                    break;
                }
//                case SDL_KEYDOWN: {
//                    SDL_Keycode key = event.key.keysym.sym;
//                    printf("KeyDown: %s", SDL_GetKeyName(key));
//                    // keyDown(key); TODO 仅功能键
//                    break;
//                }
            }
        }
    }
}
