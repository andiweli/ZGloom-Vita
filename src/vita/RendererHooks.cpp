#include "RendererHooks.h"
#include <SDL.h>
#include "../config.h"
#include "../ConfigOverlays.h"
#include "VignetteOverlay.h"
#include "FilmGrainOverlay.h"
#include "ScanlineOverlay.h"

// Discrete 0..4 -> normalized mappings (grippier where useful)
static inline float map_strength_04(int v) {
    static const float lut[5] = {0.00f, 0.25f, 0.50f, 0.80f, 1.00f};
    if (v < 0) v = 0; if (v > 4) v = 4; return lut[v];
}
static inline float map_radius_04(int v) {
    static const float lut[5] = {0.45f, 0.55f, 0.65f, 0.75f, 0.85f};
    if (v < 0) v = 0; if (v > 4) v = 4; return lut[v];
}
static inline float map_softness_04(int v) {
    static const float lut[5] = {0.15f, 0.25f, 0.35f, 0.50f, 0.70f};
    if (v < 0) v = 0; if (v > 4) v = 4; return lut[v];
}
// Film grain intensity: MUCH gentler curve requested
// 0 -> 0.00, 1 -> 0.12, 2 -> 0.22, 3 -> 0.38, 4 -> 0.55
static inline float map_grainI_04(int v) {
    static const float lut[5] = {0.00f, 0.12f, 0.22f, 0.38f, 0.55f};
    if (v < 0) v = 0; if (v > 4) v = 4; return lut[v];
}
static inline float map_scanI_04(int v) {
    static const float lut[5] = {0.00f, 0.25f, 0.50f, 0.75f, 1.00f};
    if (v < 0) v = 0; if (v > 4) v = 4; return lut[v];
}

static SDL_Renderer* gRenderer = nullptr;
static FrameLimiter  gLimiter;
static int           gTargetFps = 60;
static int           gScreenW = 960;
static int           gScreenH = 544;

static VignetteOverlay gVignette;
static FilmGrainOverlay gFilmGrain;
static ScanlineOverlay  gScanlines;
static int gFrameCounter = 0;

namespace RendererHooks {

bool init(SDL_Renderer* renderer, int screenW, int screenH) {
    gRenderer = renderer;
    gScreenW = screenW;
    gScreenH = screenH;

    LensFlareSystem::init(renderer);

    // Defaults; values are updated every frame
    gVignette.init(renderer, gScreenW, gScreenH, 0.45f, 0.65f, 0.35f, 230, 225, 255);
    gFilmGrain.init(renderer, gScreenW, gScreenH);
    gScanlines.init(renderer, gScreenW, gScreenH);
    return true;
}

void shutdown() {
    gVignette.destroy();
    gFilmGrain.destroy();
    gScanlines.destroy();
    LensFlareSystem::shutdown();
    gRenderer = nullptr;
}

void beginFrame() {
    gLimiter.beginFrame();
    LensFlareSystem::clear();
    ++gFrameCounter;
}

void endFramePresent() {
    if (gRenderer) {
        LensFlareSystem::render(gRenderer, gScreenW, gScreenH);

        // Vignette (0..4 inputs)
        int s04 = Config::GetVignetteStrength(); if (s04 < 0) s04 = 0; if (s04 > 4) s04 = 4;
        int r04 = Config::GetVignetteRadius();   if (r04 < 0) r04 = 0; if (r04 > 4) r04 = 4;
        int f04 = Config::GetVignetteSoftness(); if (f04 < 0) f04 = 0; if (f04 > 4) f04 = 4;

        gVignette.setEnabled(Config::GetVignetteEnabled() != 0);
        gVignette.setStrength(map_strength_04(s04));
        gVignette.setRadius  (map_radius_04(r04));
        gVignette.setSoftness(map_softness_04(f04));

        // Warmth
        int warmth = Config::GetVignetteWarmth();
        if (warmth < -100) warmth = -100; if (warmth > 100) warmth = 100;
        auto lerpU8 = [](float a, float b, float t){ return (Uint8)(a + (b-a)*t + 0.5f); };
        float tWarm = (warmth + 100) / 200.0f;
        const Uint8 coolR=220, coolG=230, coolB=255;
        const Uint8 warmR=255, warmG=235, warmB=220;
        const Uint8 R = lerpU8(coolR, warmR, tWarm);
        const Uint8 G = lerpU8(coolG, warmG, tWarm);
        const Uint8 B = lerpU8(coolB, warmB, tWarm);
        gVignette.setTint(R,G,B);
        gVignette.render(gRenderer);

        // Film grain (MUCH gentler mapping, slower update to reduce flicker perception)
        gFilmGrain.setEnabled(Config::GetFilmGrain() != 0);
        gFilmGrain.setParams(map_grainI_04(Config::GetFilmGrainIntensity()), 3); // update every 3 frames
        gFilmGrain.render(gRenderer, gFrameCounter);

        // Scanlines
        gScanlines.setEnabled(Config::GetScanlines() != 0);
        gScanlines.setParams(map_scanI_04(Config::GetScanlineIntensity()), 2);
        gScanlines.render(gRenderer);

        SDL_RenderPresent(gRenderer);
    }
    gLimiter.endFrameAndSleep(gTargetFps);
}

void setTargetFps(int fps) {
    gTargetFps = (fps > 0) ? fps : 60;
}

} // namespace RendererHooks

void setVignetteLevel(int lvl){
    if (lvl < 0) lvl = 0; if (lvl > 4) lvl = 4;
    Config::SetVignetteEnabled(lvl > 0 ? 1 : 0);
    Config::SetVignetteStrength(lvl);
    // keep radius/softness proportional if not configured elsewhere
    Config::SetVignetteRadius(lvl);
    Config::SetVignetteSoftness(lvl);
}
void setScanlineLevel(int lvl){
    if (lvl < 0) lvl = 0; if (lvl > 4) lvl = 4;
    Config::SetScanlines(lvl > 0 ? 1 : 0);
    Config::SetScanlineIntensity(lvl);
}
void setFilmGrainLevel(int lvl){
    if (lvl < 0) lvl = 0; if (lvl > 4) lvl = 4;
    Config::SetFilmGrain(lvl > 0 ? 1 : 0);
    Config::SetFilmGrainIntensity(lvl);
}
