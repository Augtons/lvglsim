//
// Created by Augtons on 2023/7/12.
//

#ifndef LVGLSIM_TIMER_HPP
#define LVGLSIM_TIMER_HPP

#include "SDL2/SDL.h"

class SysTimer {
private:
    SDL_TimerID timer_id = 0;
public:
    explicit SysTimer() = default;

    SysTimer(uint32_t interval, SDL_TimerCallback callback, void *param = nullptr);

    ~SysTimer();

    SysTimer(const SysTimer&) = delete;
    SysTimer& operator=(const SysTimer&) = delete;

    SysTimer(SysTimer&& other) noexcept;
    SysTimer& operator=(SysTimer&& other) noexcept;

    void remove();
};

#endif //LVGLSIM_TIMER_HPP
