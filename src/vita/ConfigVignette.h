#pragma once

// Lightweight augmentation header: declare vignette toggle accessors.
// Include this in translation units that use Config::GetVignette/SetVignette.
// Implementations live in your existing config.cpp.

namespace Config {
    int  GetVignette();
    void SetVignette(int s);
} // namespace Config
