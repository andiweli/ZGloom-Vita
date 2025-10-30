#pragma once
#include <SDL2/SDL.h>

// Lightweight "shader-like" post effect without GPU shaders.
// Renders a full-screen radial vignette with optional color tint via SDL's MOD blend.
// Cheap, stable on Vita, and easy to toggle.
class VignetteOverlay {
public:
    VignetteOverlay();
    ~VignetteOverlay();

    // Create (or recreate) the vignette texture for the given output size.
    // strength: 0..1 (how dark the edges become)
    // radius:   0..1 (radius of the bright center region as fraction of min(screenW,screenH)/2)
    // softness: 0..1 (falloff softness; higher = smoother)
    // tintRGB:  optional color tint applied at edges (e.g., {220, 210, 255} for cool mood)
    bool init(SDL_Renderer* renderer, int screenW, int screenH,
              float strength = 0.45f, float radius = 0.65f, float softness = 0.35f,
              Uint8 tintR = 255, Uint8 tintG = 240, Uint8 tintB = 230);

    void destroy();

    // Rebuild with new params, keeping the same renderer/screen size
    bool rebuild(float strength, float radius, float softness,
                 Uint8 tintR, Uint8 tintG, Uint8 tintB);

    // Draws the overlay covering the full screen (call after your scene is rendered).
    void render(SDL_Renderer* renderer);

    // Enable/disable without destroying
    void setEnabled(bool e) { enabled = e; }
    bool isEnabled() const { return enabled; }

private:
    SDL_Texture* tex = nullptr;
    int w = 0, h = 0;
    float m_strength = 0.45f;
    float m_radius   = 0.65f;
    float m_softness = 0.35f;
    Uint8 m_tintR = 255, m_tintG = 240, m_tintB = 230;
    bool enabled = true;

    bool buildTexture(SDL_Renderer* renderer);
};
