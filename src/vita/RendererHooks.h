#pragma once
#include <SDL2/SDL.h>
#include "FrameLimiter.h"
#include "LensFlare.h"

// Drop-in hooks to standardize frame flow on Vita.
// 1) Call RendererHooks::init(renderer, width, height) after creating your SDL_Renderer
// 2) At the start of each frame, call RendererHooks::beginFrame()
// 3) Draw your scene as usual onto SDL_Renderer
// 4) Before Present, submit any lens flares via LensFlareSystem::addLightScreenSpace(...)
// 5) Call RendererHooks::endFramePresent() which renders flares + vsync/limit to 60 fps
namespace RendererHooks {
    bool init(SDL_Renderer* renderer, int screenW, int screenH);
    void shutdown();
    void beginFrame();
    void endFramePresent();
    void setTargetFps(int fps);
}
