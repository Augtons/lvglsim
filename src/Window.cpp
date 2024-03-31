#include <stdexcept>
#include "Window.hpp"

lvglsim::Window::Window(const char *title, int width, int height):
    width(width), height(height)
{
    if (width < 0 || height < 0) {
        throw std::runtime_error("width and height must > 0");
    }

    // 1. Init Window
    auto window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        throw std::runtime_error(SDL_GetError());
    }
    main_window = lvglsim::unique_ptr<SDL_Window>(window, SDL_DestroyWindow);

    auto renderer = SDL_CreateRenderer((SDL_Window*)window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        throw std::runtime_error(SDL_GetError());
    }
    main_renderer = lvglsim::unique_ptr<SDL_Renderer>(renderer, SDL_DestroyRenderer);

    auto texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_TARGET, width, height);
    if (!texture) {
        throw std::runtime_error(SDL_GetError());
    }
    main_texture = lvglsim::unique_ptr<SDL_Texture>(texture, SDL_DestroyTexture);

    // 2. Init LVGL Display
    const_cast<lv_color_t*&>(display_buffer) = new lv_color_t[height * width];
    const_cast<lv_color_t*&>(lvgl_buffer_1) = new lv_color_t[width * 20 * sizeof(lv_color_t)];
    const_cast<lv_color_t*&>(lvgl_buffer_2) = new lv_color_t[width * 20 * sizeof(lv_color_t)];

    lv_disp_draw_buf_init(&disp_buf, lvgl_buffer_1, lvgl_buffer_2, width * 20 * sizeof(lv_color_t));
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &disp_buf;
    disp_drv.hor_res = (lv_coord_t)width;
    disp_drv.ver_res = (lv_coord_t)height;
    disp_drv.user_data = (void*)this;
    disp_drv.flush_cb = [](lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p) {
        auto self = (Window*)drv->user_data;
        self->flushToWindow(drv, area, color_p);
    };
    main_disp = lv_disp_drv_register(&disp_drv);

    // 3. Init LVGL Input Devices
    lv_indev_drv_init(&mouse_indev_drv);
    mouse_indev_drv.disp = main_disp;
    mouse_indev_drv.type = LV_INDEV_TYPE_POINTER;
    mouse_indev_drv.user_data = (void*)this;
    mouse_indev_drv.read_cb = [](lv_indev_drv_t * drv, lv_indev_data_t * data) {
        auto self = (Window*)drv->user_data;
        auto& controller = lvglsimIndevController::instance();
        auto mouse = controller.getMouseInputStatus(self->getWindowId());
        data->state = mouse.mouse_pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
        data->point.x = (lv_coord_t)mouse.mouse_x;
        data->point.y = (lv_coord_t)mouse.mouse_y;
    };
    mouse_indev = lv_indev_drv_register(&mouse_indev_drv);
}

lvglsim::Window::~Window()
{
    if (mouse_indev != nullptr) {
        lv_indev_delete(mouse_indev);
        mouse_indev = nullptr;
    }
    if (main_disp != nullptr) {
        lv_disp_remove(main_disp);
        main_disp = nullptr;
    }
    main_texture = std::nullopt;
    main_renderer = std::nullopt;
    main_window = std::nullopt;
    delete[] display_buffer;
    delete[] lvgl_buffer_1;
    delete[] lvgl_buffer_2;
}

void lvglsim::Window::flushToWindow(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p) {
    for(int y = 0; y <= area->y2 - area->y1; y++) {
        for(int x = 0; x <= area->x2 - area->x1; x++) {
            point_at(x, y).full = color_p->full;
            color_p++;
        }
    }

    SDL_Rect rect = {area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1};
    SDL_UpdateTexture(main_texture->get(), &rect, display_buffer, width * (int)sizeof(lv_color_t));

    if (lv_disp_flush_is_last(drv)) {
        SDL_RenderCopy(main_renderer->get(), main_texture->get(), nullptr, nullptr);
        SDL_RenderPresent(main_renderer->get());
    }

    lv_disp_flush_ready(drv);
}
