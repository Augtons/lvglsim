#include "SDL2/SDL.h"
#include "lvglsimIndevController.hpp"

void lvglsim::lvglsimIndevController::handleInputEvent(const SDL_Event& event)
{
    switch (event.type) {
        case SDL_MOUSEBUTTONDOWN: [[fallthrough]];
        case SDL_MOUSEMOTION: {
            int x, y;
            if (SDL_GetMouseState(&x, &y) & 0x01) {
                mousePress(event.motion.windowID, x, y);
            } else {
                mouseRelease(event.motion.windowID);
            }
            break;
        }
        case SDL_MOUSEBUTTONUP: {
            mouseRelease(event.key.windowID);
            break;
        }
    }
}

void lvglsim::lvglsimIndevController::erase(uint32_t windowId)
{
    mouseInputStatuses.erase(windowId);
}

auto lvglsim::lvglsimIndevController::getMouseInputStatus(uint32_t windowId) -> MouseInputStatus
{
    auto& mouse = mouseInputStatuses[windowId];
    std::lock_guard loc { mouse.lock };
    return mouse.data;
}

void lvglsim::lvglsimIndevController::mousePress(uint32_t windowId, int x, int y)
{
    auto& mouse = mouseInputStatuses[windowId];
    std::lock_guard loc { mouse.lock };
    mouse.data.mouse_pressed = true;
    mouse.data.mouse_x = x;
    mouse.data.mouse_y = y;
}

void lvglsim::lvglsimIndevController::mouseRelease(uint32_t windowId) {
    auto& mouse = mouseInputStatuses[windowId];
    std::lock_guard loc { mouse.lock };
    mouse.data.mouse_pressed = false;
}