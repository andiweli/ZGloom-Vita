#include "LensFlare.h"
#include <math.h>

SDL_Texture* LensFlareSystem::sFlareTex = nullptr;
std::vector<LensFlareSystem::Light> LensFlareSystem::sLights;

static void putPixel(SDL_Surface* s, int x, int y, Uint32 c) {
    if (x < 0 || y < 0 || x >= s->w || y >= s->h) return;
    Uint32* p = (Uint32*)((Uint8*)s->pixels + y * s->pitch) + x;
    *p = c;
}

bool LensFlareSystem::createFlareTexture(SDL_Renderer* renderer, int size) {
    // Create a simple radially faded disc (soft edge)
    SDL_Surface* surf = SDL_CreateRGBSurfaceWithFormat(0, size, size, 32, SDL_PIXELFORMAT_RGBA32);
    if (!surf) return false;
    const float cx = (float)size * 0.5f;
    const float cy = (float)size * 0.5f;
    const float r  = (float)size * 0.48f;

    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            float dx = (x - cx);
            float dy = (y - cy);
            float d = sqrtf(dx*dx + dy*dy);
            float a = 0.0f;
            if (d < r) {
                float t = 1.0f - (d / r);
                // smoother falloff
                a = t * t;
            }
            Uint8 alpha = (Uint8)(a * 255.0f);
            Uint32 color = (alpha << 24) | 0xFFFFFF; // white with alpha
            putPixel(surf, x, y, color);
        }
    }
    sFlareTex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_SetTextureBlendMode(sFlareTex, SDL_BLENDMODE_ADD);
    SDL_FreeSurface(surf);
    return sFlareTex != nullptr;
}

bool LensFlareSystem::init(SDL_Renderer* renderer) {
    if (sFlareTex) return true;
    return createFlareTexture(renderer, 64);
}

void LensFlareSystem::shutdown() {
    if (sFlareTex) {
        SDL_DestroyTexture(sFlareTex);
        sFlareTex = nullptr;
    }
    sLights.clear();
}

void LensFlareSystem::addLightScreenSpace(int x, int y, float intensity, float scale) {
    if (intensity <= 0.0f) return;
    sLights.push_back({x, y, intensity, scale});
}

void LensFlareSystem::clear() {
    sLights.clear();
}

void LensFlareSystem::render(SDL_Renderer* renderer, int screenW, int screenH) {
    if (!sFlareTex || sLights.empty()) return;
    // Additive blend already set on texture.
    for (const auto& L : sLights) {
        int texW = 0, texH = 0;
        SDL_QueryTexture(sFlareTex, nullptr, nullptr, &texW, &texH);
        const int w = (int)(texW * L.scale);
        const int h = (int)(texH * L.scale);
        SDL_Rect dst = { L.x - w/2, L.y - h/2, w, h };
        // Clamp to screen (optional)
        if (dst.x > screenW || dst.y > screenH || dst.x + dst.w < 0 || dst.y + dst.h < 0) continue;
        SDL_SetTextureAlphaMod(sFlareTex, (Uint8)(SDL_clamp(L.intensity, 0.0f, 1.0f) * 255));
        SDL_RenderCopy(renderer, sFlareTex, nullptr, &dst);
    }
}
