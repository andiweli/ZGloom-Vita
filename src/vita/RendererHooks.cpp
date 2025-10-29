#include "RendererHooks.h"
#include <SDL.h>

static SDL_Renderer* gRenderer = nullptr;
static FrameLimiter  gLimiter;
static int           gTargetFps = 60;
static int           gScreenW = 960;
static int           gScreenH = 544;

namespace RendererHooks {
bool init(SDL_Renderer* renderer, int screenW, int screenH) {
    gRenderer = renderer;
    gScreenW = screenW;
    gScreenH = screenH;
    LensFlareSystem::init(renderer);
    return true;
}

void shutdown() {
    LensFlareSystem::shutdown();
    gRenderer = nullptr;
}

void beginFrame() {
    gLimiter.beginFrame();
    // Clear optional (keep your own clear if you want special background)
    // SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    // SDL_RenderClear(gRenderer);
    LensFlareSystem::clear();
}

void endFramePresent() {
    // Render flares atop the already rendered world
    if (gRenderer) {
        LensFlareSystem::render(gRenderer, gScreenW, gScreenH);
        SDL_RenderPresent(gRenderer);
    }
    gLimiter.endFrameAndSleep(gTargetFps);
}

void setTargetFps(int fps) {
    gTargetFps = (fps > 0) ? fps : 60;
}
} // namespace RendererHooks
