#include "ConfigOverlays.h"
namespace MenuAdapters {
    int GetWarmth01() {
        int w = Config::GetVignetteWarmth(); // -100..100
        if (w < -100) w = -100; if (w > 100) w = 100;
        return (w + 100) / 20; // -> 0..10
    }
    void SetWarmth01(int v) {
        if (v < 0) v = 0; if (v > 10) v = 10;
        Config::SetVignetteWarmth(v * 20 - 100);
    }
}
