#pragma once
#include <SDL2/SDL.h>
#include <vector>

// Minimal lens flare system for SDL2 with additive blending.
// Render as screen-space sprites (quads) at provided positions.
// Integrate by: (1) call LensFlareSystem::init(renderer) after SDL_CreateRenderer
// (2) feed visible light positions via addLightScreenSpace(x,y,intensity,scale)
// (3) call render() AFTER the 3D/scene render, before Present
// (4) call clear() each frame to reset submitted lights.
//
// If you can do occlusion tests, only submit lights that are visible.
class LensFlareSystem {
public:
    struct Light {
        int x;
        int y;
        float intensity; // 0..1
        float scale;     // relative size
    };

    static bool init(SDL_Renderer* renderer);
    static void shutdown();
    static void addLightScreenSpace(int x, int y, float intensity = 1.0f, float scale = 1.0f);
    static void clear();
    static void render(SDL_Renderer* renderer, int screenW, int screenH);

private:
    static SDL_Texture* sFlareTex; // RGBA flare sprite
    static std::vector<Light> sLights;
    static bool createFlareTexture(SDL_Renderer* renderer, int size = 64);
};
