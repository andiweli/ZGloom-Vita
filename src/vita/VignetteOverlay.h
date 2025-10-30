#pragma once
#include <SDL2/SDL.h>

// Fast vignette + tint overlay (CPU), optimized for Vita.
// - Generates a HALF-RES texture and scales to screen -> ~4x fewer pixels to compute.
// - Live param setters only rebuild when values actually change (no per-frame stall).
class VignetteOverlay {
public:
    VignetteOverlay();
    ~VignetteOverlay();

    // strength/radius/softness in 0..1; tint RGB 0..255
    bool init(SDL_Renderer* renderer, int screenW, int screenH,
              float strength = 0.45f, float radius = 0.65f, float softness = 0.35f,
              Uint8 tintR = 255, Uint8 tintG = 240, Uint8 tintB = 230);

    void destroy();

    // Draw (rebuilds if params changed)
    void render(SDL_Renderer* renderer);

    // Enable/disable
    void setEnabled(bool e) { enabled = e; }
    bool isEnabled() const { return enabled; }

    // ---- Live tuning (mark dirty ONLY on change) ----
    void setStrength(float v){ if (v != m_strength){ m_strength = v; dirty = true; } }
    void setRadius  (float v){ if (v != m_radius  ){ m_radius   = v; dirty = true; } }
    void setSoftness(float v){ if (v != m_softness){ m_softness = v; dirty = true; } }
    void setTint(Uint8 r, Uint8 g, Uint8 b){
        if (r!=m_tintR || g!=m_tintG || b!=m_tintB){ m_tintR=r; m_tintG=g; m_tintB=b; dirty = true; }
    }

private:
    SDL_Texture* tex = nullptr;
    int screenW = 0, screenH = 0;
    int texW = 0, texH = 0;   // half-res texture
    float m_strength = 0.45f;
    float m_radius   = 0.65f;
    float m_softness = 0.35f;
    Uint8 m_tintR = 255, m_tintG = 240, m_tintB = 230;
    bool enabled = true;
    bool dirty = true;

    bool buildTexture(SDL_Renderer* renderer);
};
