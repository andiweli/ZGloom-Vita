#include <cstdlib>
#include <vector>
#include <string>
#include "ConfigOverlays.h"
#include "config.h"
#include <cstdio>
#include <cstring>

namespace Config {
    static int g_loaded   = 0;
    static int g_vEnabled = 0;
    static int g_vStrength = 0; // defaults <= 4 as requested
    static int g_vRadius = 0;
    static int g_vSoftness = 0;
    static int g_vWarmth  = 0; // -100..100
    static int g_grain    = 0;
    static int g_grainInt = 1; // 0..4, with 1 already strong
    static int g_scan     = 0;
    static int g_scanInt  = 1; // 0..4

    static const char* kPath = "ux0:/data/ZGloom/config.txt";

    static inline int clamp(int v, int lo, int hi){ if (v<lo) return lo; if (v>hi) return hi; return v; }

    static void ensureLoaded() {
if (g_loaded) return;
    g_loaded = true;

    std::FILE* f = std::fopen(kPath, "r");
    if (!f) {
        f = std::fopen("ux0:/data/ZGloom/config_effects.txt", "r");
        if (!f) return;
    }

    {
        char buf[512];
        while (std::fgets(buf, sizeof(buf), f)) {
            int n = (int)std::strlen(buf);
            while (n > 0 && (buf[n-1] == '\n' || buf[n-1] == '\r')) {
                buf[--n] = '\0';
            }
            char* eq = std::strchr(buf, '=');
            if (!eq) continue;
            *eq = '\0';
            const char* key = buf;
            const char* valstr = eq + 1;
            while (*valstr == ' ' || *valstr == '\t') ++valstr;
            int val = std::atoi(valstr);

            if      (!std::strcmp(key, "VIGNETTE"))   g_vEnabled  = (val != 0);
            else if (!std::strcmp(key, "V_STRENGTH")) g_vStrength = clamp(val, 0, 5);
            else if (!std::strcmp(key, "V_RADIUS"))   g_vRadius   = clamp(val, 0, 4);
            else if (!std::strcmp(key, "V_SOFTNESS")) g_vSoftness = clamp(val, 0, 5);
            else if (!std::strcmp(key, "V_WARMTH"))   g_vWarmth   = clamp(val, -100, 100);
            else if (!std::strcmp(key, "GRAIN"))      g_grain     = (val != 0);
            else if (!std::strcmp(key, "GRAIN_I") && std::strcmp(key, "GRAIN_INTENSITY"))    g_grainInt  = clamp(val, 0, 5);
            else if (!std::strcmp(key, "SCAN"))       g_scan      = (val != 0);
            else if (!std::strcmp(key, "SCAN_I")  && std::strcmp(key, "SCAN_INTENSITY"))     g_scanInt   = clamp(val, 0, 5);
        }
    }
    std::fclose(f);
}


    void EffectsConfigSave() {
    ensureLoaded();
    Save();
}

    void EffectsConfigInit(){ ensureLoaded(); }

    int  GetVignetteEnabled(){ ensureLoaded(); return g_vEnabled ? 1 : 0; }
    void SetVignetteEnabled(int s){ ensureLoaded(); g_vEnabled = (s!=0); EffectsConfigSave(); }

    int  GetVignetteStrength(){ ensureLoaded(); return g_vStrength; }
    void SetVignetteStrength(int s){ ensureLoaded(); g_vStrength = clamp(s,0,5); EffectsConfigSave(); }

    int  GetVignetteRadius(){ ensureLoaded(); return g_vRadius; }
    void SetVignetteRadius(int s){ ensureLoaded(); g_vRadius = clamp(s,0,5); EffectsConfigSave(); }

    int  GetVignetteSoftness(){ ensureLoaded(); return g_vSoftness; }
    void SetVignetteSoftness(int s){ ensureLoaded(); g_vSoftness = clamp(s,0,5); EffectsConfigSave(); }

    int  GetVignetteWarmth(){ ensureLoaded(); return g_vWarmth; }
    void SetVignetteWarmth(int s){ ensureLoaded(); g_vWarmth = clamp(s,-100,100); EffectsConfigSave(); }

    int  GetFilmGrain(){ ensureLoaded(); return g_grain ? 1 : 0; }
    void SetFilmGrain(int s){ ensureLoaded(); g_grain = (s!=0); EffectsConfigSave(); }

    int  GetFilmGrainIntensity(){ ensureLoaded(); return g_grainInt; }
    void SetFilmGrainIntensity(int s){ ensureLoaded(); g_grainInt = clamp(s,0,5); EffectsConfigSave(); }

    int  GetScanlines(){ ensureLoaded(); return g_scan ? 1 : 0; }
    void SetScanlines(int s){ ensureLoaded(); g_scan = (s!=0); EffectsConfigSave(); }

    int  GetScanlineIntensity(){ ensureLoaded(); return g_scanInt; }
    void SetScanlineIntensity(int s){ ensureLoaded(); g_scanInt = clamp(s,0,5); EffectsConfigSave(); }
} // namespace Config
