#include "VignetteOverlay.h"
#include <algorithm>
#include <cmath>

static inline float clamp01(float x){ return x < 0.f ? 0.f : (x > 1.f ? 1.f : x); }

VignetteOverlay::VignetteOverlay() {}
VignetteOverlay::~VignetteOverlay() { destroy(); }

bool VignetteOverlay::init(SDL_Renderer* renderer, int screenW, int screenH,
                           float strength, float radius, float softness,
                           Uint8 tintR, Uint8 tintG, Uint8 tintB)
{
    destroy();
    w = screenW;
    h = screenH;
    m_strength = clamp01(strength);
    m_radius   = clamp01(radius);
    m_softness = clamp01(softness);
    m_tintR = tintR; m_tintG = tintG; m_tintB = tintB;
    return buildTexture(renderer);
}

void VignetteOverlay::destroy()
{
    if (tex) {
        SDL_DestroyTexture(tex);
        tex = nullptr;
    }
}

bool VignetteOverlay::rebuild(float strength, float radius, float softness,
                              Uint8 tintR, Uint8 tintG, Uint8 tintB)
{
    m_strength = clamp01(strength);
    m_radius   = clamp01(radius);
    m_softness = clamp01(softness);
    m_tintR = tintR; m_tintG = tintG; m_tintB = tintB;
    // Caller must pass a renderer to build; handled via init or render guard.
    return tex != nullptr; // actual rebuild happens in buildTexture when render() called with a renderer
}

bool VignetteOverlay::buildTexture(SDL_Renderer* renderer)
{
    if (!renderer || w <= 0 || h <= 0) return false;

    // Build a texture at screen resolution for best quality (still cheap: ~960x544).
    tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, w, h);
    if (!tex) return false;

    // We want multiplicative blending (dst = src * dst), use MOD
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_MOD);

    void* pixels = nullptr;
    int pitch = 0;
    if (SDL_LockTexture(tex, nullptr, &pixels, &pitch) != 0) {
        SDL_DestroyTexture(tex); tex = nullptr;
        return false;
    }

    // Precompute center/radius in pixels
    const float cx = (w - 1) * 0.5f;
    const float cy = (h - 1) * 0.5f;
    const float minHalf = std::min(w, h) * 0.5f;
    const float inner = m_radius * minHalf;
    const float soft  = std::max(1.0f, m_softness * minHalf); // px
    const float invSoft = 1.0f / soft;

    // Fill pixels: edge darkening factor using smoothstep between inner and inner+soft
    for (int y = 0; y < h; ++y) {
        Uint32* row = reinterpret_cast<Uint32*>(static_cast<Uint8*>(pixels) + y * pitch);
        for (int x = 0; x < w; ++x) {
            const float dx = x - cx;
            const float dy = y - cy;
            const float r  = std::sqrt(dx*dx + dy*dy);

            float t = (r - inner) * invSoft;    // <0 inside inner radius
            t = clamp01(t);
            // smoothstep
            t = t * t * (3.f - 2.f * t);

            // Vignette factor: 1 - strength * t  (1 at center, 1-strength at far edge)
            const float f = clamp01(1.f - m_strength * t);

            // Apply optional color tint towards edges by mixing with tint color via MOD:
            // We encode the modulation color per pixel. Center near 255, edges approach tintRGB * f.
            const Uint8 R = static_cast<Uint8>(std::round((m_tintR * f) + (255 - 255 * f)));
            const Uint8 G = static_cast<Uint8>(std::round((m_tintG * f) + (255 - 255 * f)));
            const Uint8 B = static_cast<Uint8>(std::round((m_tintB * f) + (255 - 255 * f)));

            // Pack ABGR (alpha ignored for MOD; keep 255)
            row[x] = (255u << 24) | (B << 16) | (G << 8) | (R);
        }
    }

    SDL_UnlockTexture(tex);
    return true;
}

void VignetteOverlay::render(SDL_Renderer* renderer)
{
    if (!enabled) return;
    if (!renderer) return;
    if (!tex) {
        // (Re)build if needed
        if (!buildTexture(renderer)) return;
    }
    SDL_Rect dst{0,0,w,h};
    SDL_RenderCopy(renderer, tex, nullptr, &dst);
}
