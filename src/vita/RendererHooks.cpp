#include "RendererHooks.h"
#include <SDL.h>
#include "../config.h"   // relative path: src/vita -> src
#include "VignetteOverlay.h" // both in src/vita/

namespace Config { int GetVignette(); void SetVignette(int); } // forward decls

static SDL_Renderer* gRenderer = nullptr;
static FrameLimiter  gLimiter;
static int           gTargetFps = 60;
static int           gScreenW = 960;
static int           gScreenH = 544;

// shader-like post effect
static VignetteOverlay gVignette;

namespace RendererHooks {

bool init(SDL_Renderer* renderer, int screenW, int screenH) {
    gRenderer = renderer;
    gScreenW = screenW;
    gScreenH = screenH;

    // Lens flares first (scene element)
    LensFlareSystem::init(renderer);

    // Build vignette/tint overlay (top-most post effect)
    gVignette.init(renderer, gScreenW, gScreenH, 0.45f, 0.65f, 0.35f, 230, 225, 255);

    return true;
}

void shutdown() {
    // Destroy post effect first
    gVignette.destroy();

    // Then scene effects
    LensFlareSystem::shutdown();

    gRenderer = nullptr;
}

void beginFrame() {
    gLimiter.beginFrame();
    LensFlareSystem::clear();
}

void endFramePresent() {
    if (gRenderer) {
        // Render scene-attached lens flares first
        LensFlareSystem::render(gRenderer, gScreenW, gScreenH);

        // Sync with config toggle each frame
        gVignette.setEnabled(Config::GetVignette() != 0);

        // Then top-most vignette/tint overlay (multiplicative MOD blend)
        gVignette.render(gRenderer);

        SDL_RenderPresent(gRenderer);
    }
    gLimiter.endFrameAndSleep(gTargetFps);
}

void setTargetFps(int fps) {
    gTargetFps = (fps > 0) ? fps : 60;
}

} // namespace RendererHooks
