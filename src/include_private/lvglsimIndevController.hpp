#pragma once

#include <unordered_map>
#include <mutex>
#include "SDL2/SDL.h"

namespace lvglsim {
    struct MouseInputStatus {
        bool mouse_pressed = false;
        int mouse_x = 0;
        int mouse_y = 0;
    };
}

namespace lvglsim {
    class lvglsimIndevController {
    public:
        static auto instance() -> lvglsimIndevController& {
            static lvglsimIndevController ins {};
            return ins;
        }

        auto getMouseInputStatus(uint32_t windowId) -> MouseInputStatus;
        void erase(uint32_t windowId);

        void handleInputEvent(const SDL_Event& event);

    private:
        template<typename T>
        struct LockedData {
            std::mutex lock;
            T data;
        };

        std::unordered_map<uint32_t, LockedData<MouseInputStatus>> mouseInputStatuses;

        lvglsimIndevController() = default;

        void mousePress(uint32_t windowId, int x, int y);
        void mouseRelease(uint32_t windowId);
    };
}
