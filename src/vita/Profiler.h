#pragma once
#include <SDL2/SDL.h>
#include <string>

// Lightweight scoped profiler for quick timing in milliseconds.
// Usage:
//   { ScopedTimer t("Render"); /* code */ }  // prints to SDL_Log
class ScopedTimer {
public:
    ScopedTimer(const char* name) : mName(name) {
        mFreq = SDL_GetPerformanceFrequency();
        mStart = SDL_GetPerformanceCounter();
    }
    ~ScopedTimer() {
        const Uint64 end = SDL_GetPerformanceCounter();
        const double ms = 1000.0 * (double)(end - mStart) / (double)mFreq;
        SDL_Log("%s: %.3f ms", mName, ms);
    }
private:
    const char* mName;
    Uint64 mStart;
    Uint64 mFreq;
};
