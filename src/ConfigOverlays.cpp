#include "ConfigOverlays.h"
#include <cstdio>
#include <cstring>

namespace Config {
    static int g_loaded   = 0;
    static int g_vEnabled = 1;
    static int g_vStrength= 2; // defaults <= 4 as requested
    static int g_vRadius  = 2;
    static int g_vSoftness= 1;
    static int g_vWarmth  = 0; // -100..100
    static int g_grain    = 0;
    static int g_grainInt = 1; // 0..4, with 1 already strong
    static int g_scan     = 0;
    static int g_scanInt  = 1; // 0..4

    static const char* kPath = "ux0:/data/ZGloom/config_effects.txt";

    static inline int clamp(int v, int lo, int hi){ if (v<lo) return lo; if (v>hi) return hi; return v; }

    static void ensureLoaded() {
        if (g_loaded) return;
        g_loaded = 1;
        std::FILE* f = std::fopen(kPath, "r");
        if (!f) return;
        char key[64]; int val=0;
        while (std::fscanf(f, "%63[^=]=%d\n", key, &val) == 2) {
            if      (!std::strcmp(key, "VIGNETTE")) g_vEnabled  = (val!=0);
            else if (!std::strcmp(key, "V_STRENGTH")) g_vStrength = clamp(val,0,4);
            else if (!std::strcmp(key, "V_RADIUS"))   g_vRadius   = clamp(val,0,4);
            else if (!std::strcmp(key, "V_SOFTNESS")) g_vSoftness = clamp(val,0,4);
            else if (!std::strcmp(key, "V_WARMTH"))   g_vWarmth   = clamp(val,-100,100);
            else if (!std::strcmp(key, "GRAIN"))      g_grain     = (val!=0);
            else if (!std::strcmp(key, "GRAIN_I"))    g_grainInt  = clamp(val,0,4);
            else if (!std::strcmp(key, "SCAN"))       g_scan      = (val!=0);
            else if (!std::strcmp(key, "SCAN_I"))     g_scanInt   = clamp(val,0,4);
            // ignore unknowns
        }
        std::fclose(f);
    }

    void EffectsConfigSave() {
        std::FILE* f = std::fopen(kPath, "w");
        if (!f) return;
        std::fprintf(f, "VIGNETTE=%d\n", g_vEnabled ? 1 : 0);
        std::fprintf(f, "V_STRENGTH=%d\n", g_vStrength);
        std::fprintf(f, "V_RADIUS=%d\n", g_vRadius);
        std::fprintf(f, "V_SOFTNESS=%d\n", g_vSoftness);
        std::fprintf(f, "V_WARMTH=%d\n", g_vWarmth);
        std::fprintf(f, "GRAIN=%d\n", g_grain ? 1 : 0);
        std::fprintf(f, "GRAIN_I=%d\n", g_grainInt);
        std::fprintf(f, "SCAN=%d\n", g_scan ? 1 : 0);
        std::fprintf(f, "SCAN_I=%d\n", g_scanInt);
        std::fclose(f);
    }

    void EffectsConfigInit(){ ensureLoaded(); }

    int  GetVignetteEnabled(){ ensureLoaded(); return g_vEnabled ? 1 : 0; }
    void SetVignetteEnabled(int s){ ensureLoaded(); g_vEnabled = (s!=0); EffectsConfigSave(); }

    int  GetVignetteStrength(){ ensureLoaded(); return g_vStrength; }
    void SetVignetteStrength(int s){ ensureLoaded(); g_vStrength = clamp(s,0,4); EffectsConfigSave(); }

    int  GetVignetteRadius(){ ensureLoaded(); return g_vRadius; }
    void SetVignetteRadius(int s){ ensureLoaded(); g_vRadius = clamp(s,0,4); EffectsConfigSave(); }

    int  GetVignetteSoftness(){ ensureLoaded(); return g_vSoftness; }
    void SetVignetteSoftness(int s){ ensureLoaded(); g_vSoftness = clamp(s,0,4); EffectsConfigSave(); }

    int  GetVignetteWarmth(){ ensureLoaded(); return g_vWarmth; }
    void SetVignetteWarmth(int s){ ensureLoaded(); g_vWarmth = clamp(s,-100,100); EffectsConfigSave(); }

    int  GetFilmGrain(){ ensureLoaded(); return g_grain ? 1 : 0; }
    void SetFilmGrain(int s){ ensureLoaded(); g_grain = (s!=0); EffectsConfigSave(); }

    int  GetFilmGrainIntensity(){ ensureLoaded(); return g_grainInt; }
    void SetFilmGrainIntensity(int s){ ensureLoaded(); g_grainInt = clamp(s,0,4); EffectsConfigSave(); }

    int  GetScanlines(){ ensureLoaded(); return g_scan ? 1 : 0; }
    void SetScanlines(int s){ ensureLoaded(); g_scan = (s!=0); EffectsConfigSave(); }

    int  GetScanlineIntensity(){ ensureLoaded(); return g_scanInt; }
    void SetScanlineIntensity(int s){ ensureLoaded(); g_scanInt = clamp(s,0,4); EffectsConfigSave(); }
} // namespace Config
