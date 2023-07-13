#include "Timer.hpp"

SysTimer::SysTimer(uint32_t interval, SDL_TimerCallback callback, void *param) {
    timer_id = SDL_AddTimer(interval, callback, param);
}

SysTimer::~SysTimer() {
    remove();
}

SysTimer::SysTimer(SysTimer&& other) noexcept {
    timer_id = other.timer_id;
    other.timer_id = 0;
}

SysTimer& SysTimer::operator=(SysTimer&& other) noexcept {
    timer_id = other.timer_id;
    other.timer_id = 0;
    return *this;
}

void SysTimer::remove() {
    if (timer_id != 0) {
        SDL_RemoveTimer(timer_id);
        timer_id = 0;
    }
}