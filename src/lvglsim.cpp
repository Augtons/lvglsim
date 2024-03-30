#include <memory>
#include <queue>
#include <functional>
#include <thread>
#include <any>
#include <future>
#include "SDL2/SDL.h"
#include "Timer.hpp"
#include "Window.hpp"
#include "lvglsimIndevController.hpp"
#include "lvglsim.h"

/* SDL User Event Codes */
#define SDL_USER_EVENT_CODE_RUN_ON_SDL_THREAD    (1)

/* Containers */
static std::optional<uint32_t> main_window_id = std::nullopt;
static std::unordered_map<uint32_t, std::unique_ptr<lvglsim::Window>> windows {};

/* Run On SDL Thread Queue */
std::queue<std::function<std::any()>> runOnSdlThreadQueue;

/* Lock */
std::recursive_mutex lock;

template<typename R>
static auto run_on_sdl_thread(std::function<R()> func) -> R {
    runOnSdlThreadQueue.emplace([func{std::move(func)}]() -> std::any {
        return func();
    });
    std::promise<std::any> result{};

    SDL_Event event;
    event.type = SDL_USEREVENT;
    event.user.code = SDL_USER_EVENT_CODE_RUN_ON_SDL_THREAD;
    event.user.data1 = &result;

    SDL_PushEvent(&event);

    auto future = result.get_future();
    auto ret = std::any_cast<R>(future.get());

    return ret;
}

auto lvglsim_new_window(const window_config_t *config) -> uint32_t {
    static std::atomic<bool> has_inited = false;

    if (!has_inited) {
        lv_init();

        int ret = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
        if (ret < 0) {
            fprintf(stderr, "runtime_error: %s\n", SDL_GetError());
            abort();
        }
        has_inited = true;
    }

    auto create_function = [config { *config }]() -> uint32_t {
        try {
            auto window = std::make_unique<lvglsim::Window>(config.title, config.width, config.height);
            auto id = window->getWindowId();
            if (!main_window_id) {
                main_window_id = id;
            }
            windows.try_emplace(window->getWindowId(), std::move(window));
            return id;
        } catch (const std::runtime_error& e) {
            fprintf(stderr, "runtime_error: %s\n", e.what());
            abort();
        }
    };

    if (!main_window_id) {
        return create_function();
    }

    try {
        return run_on_sdl_thread<uint32_t>(std::move(create_function));
    } catch (const std::runtime_error& e) {
        fprintf(stderr, "runtime_error: %s\n", e.what());
        abort();
    }
}

auto lvglsim_get_disp(uint32_t windowId) -> lv_disp_t* {
    if (windows.find(windowId) == windows.end()) {
        return nullptr;
    }
    return windows[windowId]->getDisplay();
}

void lvglsim_lock() {
    lock.lock();
}

void lvglsim_unlock() {
    lock.unlock();
}

auto lvglsim_try_lock() -> bool {
    return lock.try_lock();
}

void displays_loop()
{
    using namespace std::chrono_literals;
    auto& controller = lvglsim::lvglsimIndevController::instance();

    auto timer = SysTimer(10, [](uint32_t interval, void*) -> uint32_t {
        lv_tick_inc(10);
        return interval;
    });

    std::atomic<bool> has_finished = false;
    auto lvgl_thread = std::thread([&] {
        while (!has_finished) {
            {
                std::lock_guard locker { lock };
                lv_task_handler();
            }
            std::this_thread::sleep_for(5ms);
        }
    });

    SDL_Event event;

    while (true) {
        auto polled = SDL_WaitEvent(&event);
        if (!polled) {
            continue;
        }

        if (event.type == SDL_QUIT) {
            break;
        }

        if (event.type == SDL_WINDOWEVENT) {
            if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
                auto windowId = event.window.windowID;
                if (windowId == main_window_id) {
                    break;
                }
                auto it = windows.find(windowId);
                if (it == windows.end()) {
                    continue;
                }
                auto& [_, window_ptr] = *it;
                std::lock_guard locker { lock };
                SDL_RestoreWindow(window_ptr->getSDLWindowPointer());  // 因为部分平台，关闭最小化窗口可能导致崩溃
                windows.erase(windowId);
                controller.erase(windowId);
                continue;
            }
            if (event.window.event == SDL_WINDOWEVENT_EXPOSED) {
                auto windowId = event.window.windowID;
                auto it = windows.find(windowId);
                if (it == windows.end()) {
                    continue;
                }
                auto& [_, window_ptr] = *it;
                auto root = lv_disp_get_scr_act(window_ptr->getDisplay());
                // 强制刷新一次全屏 (由于 lv_refr_now() 在部分平台上仍然导致画面无变化)
                std::lock_guard locker { lock };
                auto root_hide = lv_obj_has_flag(root, LV_OBJ_FLAG_HIDDEN);
                if (root_hide) {
                    lv_obj_clear_flag(root, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_add_flag(root, LV_OBJ_FLAG_HIDDEN);
                } else {
                    lv_obj_add_flag(root, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_clear_flag(root, LV_OBJ_FLAG_HIDDEN);
                }
            }
        }

        if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONUP) {
            controller.handleInputEvent(event);
            continue;
        }

        if (event.type == SDL_USEREVENT) {
            if (event.user.code == SDL_USER_EVENT_CODE_RUN_ON_SDL_THREAD) {
                auto& promise = *reinterpret_cast<std::promise<std::any>*>(event.user.data1);
                try {
                    auto ret = runOnSdlThreadQueue.front().operator()();
                    runOnSdlThreadQueue.pop();
                    promise.set_value(std::move(ret));
                } catch (...) {
                    promise.set_exception(std::current_exception());
                }
                continue;
            }
        }
    }
    has_finished = true;
    lvgl_thread.join();
}

int main(int argc, char *argv[]) {
    int ret = app_main(argc, argv);
    if (ret != 0) {
        return ret;
    }
    displays_loop();

    return 0;
}
