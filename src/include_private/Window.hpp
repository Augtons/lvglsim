#ifndef LVGLSIM_WINDOW_HPP
#define LVGLSIM_WINDOW_HPP

#include <memory>
#include <optional>
#include <mutex>
#include "SDL2/SDL.h"
#include "lvgl.h"
#include "lvglsimIndevController.hpp"

namespace lvglsim {
    template<typename T>
    using unique_ptr = std::unique_ptr<T, void(*)(T*)>;
}

namespace lvglsim {
class Window {
    public:
        Window(const char *title, int width, int height);
        ~Window();

        Window(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(const Window&) = delete;
        Window& operator=(Window&&) = delete;

        auto getSDLWindowPointer() -> SDL_Window* { return main_window->get(); }
        auto getWindowId() -> uint32_t { return SDL_GetWindowID(getSDLWindowPointer()); }
        auto getDisplay() -> lv_disp_t* { return main_disp; }

    private:
        const int width;
        const int height;

        std::optional<lvglsim::unique_ptr<SDL_Window>>     main_window;
        std::optional<lvglsim::unique_ptr<SDL_Renderer>>   main_renderer;
        std::optional<lvglsim::unique_ptr<SDL_Texture>>    main_texture;

        lv_color_t* const display_buffer = nullptr;
        lv_color_t* const lvgl_buffer_1 = nullptr;
        lv_color_t* const lvgl_buffer_2 = nullptr;

        lv_disp_draw_buf_t disp_buf {};
        lv_disp_drv_t disp_drv {};
        lv_disp_t* main_disp = nullptr;

        lv_indev_drv_t mouse_indev_drv {};
        lv_indev_t *mouse_indev = nullptr;

        void flushToWindow(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p);

        auto point_at(int x, int y) -> lv_color_t& {
            return display_buffer[y * width + x];
        }
    };
}

#endif